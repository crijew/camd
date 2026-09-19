import os
from pathlib import Path
import subprocess
import unittest

import yaml


ROOT = Path(__file__).resolve().parents[1]


def workflow(name):
    path = f".github/workflows/{name}.yml"
    ref = os.environ.get("WORKFLOW_TEST_REF")
    text = subprocess.check_output(
        ["git", "show", f"{ref}:{path}"], cwd=ROOT, text=True
    ) if ref else (ROOT / path).read_text()
    return yaml.load(text, Loader=yaml.BaseLoader)


class SyncWorkflowTests(unittest.TestCase):
    def test_ci_permissions(self):
        self.assertEqual(workflow("sync-upstream")["permissions"].get("actions"), "write")

    def publish(self, delay=0):
        steps = workflow("sync-upstream")["jobs"]["sync"]["steps"]
        script = next(step["run"] for step in steps
                      if step.get("name") == "Publish sync pull request")
        harness = r'''
SYNC_BRANCH=automation/sync-suitesparse-camd
GITHUB_REF_NAME=main
GITHUB_REPOSITORY=example/camd
remaining=DELAY
approved=false
source() { commit=abcdef123456; commit_date=2026-09-13; }
git() {
  case "$1" in
    status) echo ' M UPSTREAM' ;;
    rev-parse) echo new-head ;;
  esac
}
sleep() { remaining=$((remaining - 1)); }
gh() {
  case "$1 $2" in
    'pr list') echo https://github.com/example/camd/pull/3 ;;
    'pr view')
      if [ "$remaining" -gt 0 ]; then echo old-head; else echo new-head; fi ;;
    'pr edit') ;;
    'run list')
      [[ "$*" == *'--commit new-head --event pull_request'* ]] || return 1
      echo 123 ;;
    'run view') echo action_required ;;
    'api --method')
      [ "$3" = POST ] && [ "$4" = repos/example/camd/actions/runs/123/approve ] || return 1
      approved=true
      echo approved ;;
    'pr merge')
      [ "$approved" = true ] || { echo 'CI was not approved' >&2; return 1; }
      [ "$remaining" -le 0 ] || { echo 'PR head is stale' >&2; return 1; }
      [[ "$*" == *'--match-head-commit new-head'* ]] || return 1
      echo merged ;;
    *) return 1 ;;
  esac
}
'''.replace("DELAY", str(delay))
        return subprocess.run(["bash", "-e", "-c", harness + script],
                              text=True, capture_output=True, timeout=5)

    def test_publish_approves_before_merge(self):
        result = self.publish()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("approved\nmerged", result.stdout)

    def test_publish_waits_for_pr_head(self):
        result = self.publish(delay=2)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("merged", result.stdout)

    def test_stale_pr_head_stops_publication(self):
        result = self.publish(delay=20)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("Pull request head did not update", result.stderr)
        self.assertNotIn("approved", result.stdout)
        self.assertNotIn("merged", result.stdout)


if __name__ == "__main__":
    unittest.main()
