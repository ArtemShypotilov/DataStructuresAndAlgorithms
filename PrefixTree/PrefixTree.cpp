
struct TrieNode{
    TrieNode * childs[26];
    bool is_word_here;
    TrieNode(){
        is_word_here=false;
        for(int i=0;i<26;i++)
        {
            childs[i]=nullptr;
        }
    }

};

class Trie {
    
    
    
public:
    
    TrieNode *root;
    

    
    /** Initialize your data structure here. */
    Trie() {
        root = new TrieNode();
    }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
        TrieNode* curr=root;
        for (auto it = word.cbegin() ; it != word.cend(); ++it) {
            if (!curr->childs[*it-'a'])
                curr->childs[*it-'a'] =new TrieNode();
            curr = curr->childs[*it-'a'];
	    }
        curr->is_word_here = true;
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
        TrieNode* curr=root;
        for (auto it = word.cbegin() ; it != word.cend(); ++it) {
            if (!curr->childs[*it-'a'])
                return false;
            curr = curr->childs[*it-'a'];
	    }
        if (!curr->is_word_here) return false;
        return true;
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        TrieNode* curr=root;
        for (auto it = prefix.cbegin() ; it != prefix.cend(); ++it) {
            if (!curr->childs[*it-'a'])
                return false;
            curr = curr->childs[*it-'a'];
	    }
        return true;
    }
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */