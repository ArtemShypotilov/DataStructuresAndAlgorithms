/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode *> s; 
        TreeNode *curr = root; 
  
        while (curr != NULL || s.empty() == false) 
        { 
            /* Reach the left most Node of the 
               curr Node */
            while (curr !=  NULL) 
            { 
                /* place pointer to a tree node on 
                   the stack before traversing 
                  the node's left subtree */
                s.push(curr); 
                curr = curr->left; 
            } 

            /* Current must be NULL at this point */
            curr = s.top(); 
            s.pop(); 

            result.push_back(curr->val); 

            /* we have visited the node and its 
               left subtree.  Now, it's right 
               subtree's turn */
            curr = curr->right; 

        } /* end of while */
        return result;
    }
};