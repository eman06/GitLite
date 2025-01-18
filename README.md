# GitLite
This project implements a version control system for CSV data using tree-based data structures like AVL, B Tree, or Red-Black Tree. It supports large datasets by storing tree nodes as individual files, allowing memory-efficient operations. Key features include repository initialization, branching, committing changes, and switching branch
1. Initialize Repository (init Command)
Command: init <filename>

Description: Initializes the repository, prompting the user to upload a CSV file and choose a data structure (AVL Tree, B Tree, or Red-Black Tree).

Process:

Uploads a large CSV file.
Prompts the user to select a tree structure and (for B Tree) its order.
Allows the user to specify the column for tree organization by displaying column names from the first line of the file.
Constructs the tree, storing each node as a separate file containing node data, parent, and child references.
Performance: Optimized for large datasets by loading only necessary data into memory.

2. Commit Changes (commit Command)
Command: commit "message"
Description: Saves the current state of the tree structure, creating a new version of the repository along with a user-provided commit message.
3. Branching with Folders (branch Command)
Command: branch <branch_name>
Description: Creates a new branch stored in a separate folder, duplicating the current tree and data to allow independent modifications.
Process:
A new folder is created for the branch.
All tree nodes and files are copied to the branch folder for isolated work.
