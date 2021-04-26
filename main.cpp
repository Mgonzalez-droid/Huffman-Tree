#include <iostream>
#include <map>
#include <queue>
#include <fstream>
#include <algorithm>
class huffman_tree {
private:
    
    /*
     * NODE
     * ----
     * Classification:
     * Project spec.
     *
     * Description:
     * in order to create the huffman tree it was needed to store our huffman code into
     * a node that could be utilized when traversing the tree to find the appropriate
     * huffman encoding of a charaacter. Within the node we define the frequency and
     * char value of the node. A frequecny node for example is set to '-1' to avoid
     * any errors when creating the huffman tree.
     */
    struct node{
        char c;
        int f;
        node* l = NULL;
        node* r = NULL;
        bool parent = true;
        
        //Utilized when creating an empty node
        node(){
            
        }
        
        //utilized when creating a huffman code containg a char and frequency
        node(char c, int f){
            this -> c = c;
            this -> f = f;
            parent = false;
        }
        
        //Utilized when creating a huffman code describing just a frequency
        node(int f){
            this -> f = f;
            this -> c = '-1';
        }
    };
    
    // The root belongs to the huffman tree class and cannot be accessed outside the tree
    node* root;
    
    //Creates the root of the huffman tree
    huffman_tree(node* n){
        this -> root = n;
    }

public:

    /*
    Preconditions: input is a string of characters with ascii values 0-127
    Postconditions: Reads the characters of input and constructs a
    huffman tree based on the character frequencies of the file contents
    */
    
    /*
     * HUFFMAN_TREE
     * ------------
     * Classification:
     * Project spec.
     *
     * Description:
     * This method process both passed through strings and files by specifying
     * a file starting the string with "/". Additionally to avoid opening a
     * file when not wanted we check the size of the string to ensure the user
     * is not just passing "/". After it is verified the type of data being
     * passed we iterate through a couple loops using a sorted map to place
     * each node in the queue and create the appropriate huffman tree.
     */
    
    huffman_tree(const std::string& input) {
        std::priority_queue<std::pair<int,node*>> q;
        std::map<char,int> mappy;
        
        if(input[0] == '/' && input.size() > 1){
            try{
                std::string file = input;
                file.erase(0,1);
                std::ifstream infile(file,std::ifstream::in);
                char c = infile.get();
                
                while(infile.good()){
                    if(mappy.find(c) != mappy.end()){
                        mappy.find(c) -> second += 1;
                        c = infile.get();
                    }else{
                        mappy[c] = 1;
                        c = infile.get();
                    }
                }
                for(auto it : mappy){
                    node* n = new node(it.first, it.second);
                    std::pair<int,node*> p;
                    p.first = it.second * -1;
                    p.second = n;
                    q.push(p);
                }
                while(q.size() != 1){
                    node* n = new node;
                    std::pair<int, node*> p;
                    n -> l = q.top().second;
                    q.pop();
                    n -> r = q.top().second;
                    q.pop();
                    n -> f = n -> l -> f + n -> r -> f;
                    p.first= n->f * -1;
                    p.second=n;
                    q.push(p);
                }
                if(mappy.size() == 1){
                    root = new node(1);
                    root -> l = q.top().second;
                }else{
                    this -> root = q.top().second;
                }
            }catch(std::string e){
            }
        }else{
            for(auto c : input){
                if(mappy.find(c) != mappy.end()){
                    mappy.find(c) -> second += 1;
                }else{
                    mappy[c] = 1;
                }
            }
            for(auto it : mappy){
                node* n = new node(it.first, it.second);
                std::pair<int,node*> p;
                p.first = it.second * -1;
                p.second = n;
                q.push(p);
            }
            while(q.size() != 1){
                node* n = new node;
                std::pair<int, node*> p;
                n -> l = q.top().second;
                q.pop();
                n -> r = q.top().second;
                q.pop();
                n -> f = n -> l -> f + n -> r -> f;
                p.first= n->f * -1;
                p.second=n;
                q.push(p);
            }
            if(mappy.size() == 1){
                root = new node(1);
                root -> l = q.top().second;
            }else{
                this -> root = q.top().second;
            }
        }
    }
    
    /*
        Preconditions: input is a character with ascii value between 0-127
        Postconditions: Returns the Huffman code for character if character is in the tree
        and an empty string otherwise.
    */
    
    /*
     * SEARCH
     * ------
     * Classification:
     * Helper method
     *
     * Description:
     * This search method is used to return the huffman code of a given character, this is
     * done recursively through this method. When it encounters and issue traversing one
     * side of the tree it runs through the other side of the tree. That way the appropriate
     * ascii code is utilized to define a char.
     */
    
    std::string search(std::string code, node* n, char character) const{
        std::string oCode = code;
        if(n == NULL){
            return "error";
        } else{
            if(n -> parent){
                code.append("0");
                code = search(code,n -> l,character);
                
                if(code == "error"){
                    code = oCode;
                    code.append("1");
                    code = search(code, n -> r, character);
                }
                
                if(code == "error"){
                    return "error";
                } else{
                    return code;
                }
            }
            
            if (tolower(n -> c) == character) {
                return code;
            } else{
                if(character < n -> c ){
                    code.append("0");
                    return search(code, n -> l, character);
                } else{
                    code.append("1");
                    return search(code, n -> r, character);
                }
            }
        }
    }
    
    /*
     *  GET_CHARACTER_CODE
     * --------------------
     * Classification:
     * helper method
     *
     * DESCRIPTION:
     * This class is a helper method used by encode to encode the appropriate
     * huffman code to the chracter. This is done recursively through search
     * for incresed performance eficiency.
     */
    std::string get_character_code(char character) const {
        character = tolower(character);
        std::string code = "";
        if(root == NULL){
            return code;
        }else{
            code.append("0");
            code = search(code,root -> l,character);
            
            if(code == "error"){
                code = "";
                code.append("1");
                code = search(code, root -> r, character);
            }
            if(code == "error"){
                return "";
            } else{
                return code;
            }
        }
    }
    
    /*
    Preconditions: input is a string of characters with ascii values 0-127
    Postconditions: Returns the Huffman encoding for the contents of file_name
    if file name exists and an empty string otherwise.
    If the file contains letters not present in the huffman_tree,
    return an empty string
    */
    
    /*
     *  ENCODE
     * --------
     * Classification:
     * Project spec.
     *
     * Description:
     * This is part of the project spec its primary function is to encode the
     * string passed to it or file. This done iteratively through a helper
     * method get_character_code and its recursive helper method search.
     */
    
    std::string encode(const std::string& input) const {
        std::string result = "null";
        if(input[0] == '/' && input.size() > 1){
            try{
                std::string file = input;
                file.erase(0,1);
                std::ifstream infile(file,std::ifstream::in);
                char c = infile.get();
                
                while(infile.good() && result != ""){
                    if(result == ""){
                        return result;
                    }else if(result == "null"){
                        result = "";
                        result += get_character_code(c);
                    }else{
                        result += get_character_code(c);
                    }
                    c = infile.get();
                }
            }catch(std::string e){
                return"";
            }
        } else{
            for(char c : input){
                if(result == ""){
                    return result;
                }else if(result == "null"){
                    result = "";
                    result += get_character_code(c);
                }else{
                    result += get_character_code(c);
                }
            }
        }
        return result;
    }


    /*
    Preconditions: string_to_decode is a string containing Huffman-encoded text
    Postconditions: Returns the plaintext represented by the string if the string
    is a valid Huffman encoding and an empty string otherwise
    */
    
    /*
     * DECODE
     * ------
     * Clasification:
     * Project spec.
     *
     * Description:
     * This method iteratively searches through the huffman tree, once it
     * reaches a null chracter it returns the char character of the node.
     * this allows the user to decode the message accurately after encoding.
     */
    
    std::string decode(const std::string& string_to_decode) const {
        std::string message = "";
        node* n = root;
        for(char c : string_to_decode){
            if(c == '0' || c == '1'){
                if(c == '0'){
                    n = n -> l;
                    if(n -> l == NULL || n -> r == NULL){
                        message += n -> c;
                        n = root;
                    }
                }
                
                if(c == '1'){
                    n = n -> r;
                    if(n -> l == NULL || n -> r == NULL){
                        message += n -> c;
                        n = root;
                    }
                }
            }
        }
        return message;
    }
};