# Verification scripts

No build script is included yet because the live repository, targets, engine installation, and successful command were not available when this bundle was generated.

After `BE-0001` proves the command manually, add `verify.ps1` for the Windows project. It should:

1. Resolve paths relative to the repository where possible.
2. Validate the exact `.uproject`, target, engine version, and required executables before running.
3. Build the verified Editor target and preserve the exit code.
4. Save a timestamped log under an ignored/local evidence directory or the task's evidence folder.
5. Avoid deleting `Binaries`, `Intermediate`, `Saved`, Derived Data, or user work.
6. Fail loudly on missing prerequisites and print the exact next action.

Do not automate Unreal Editor asset mutations as part of basic verification.
