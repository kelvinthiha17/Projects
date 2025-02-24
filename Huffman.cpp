#include "Huffman.h"
#include <sstream>
using namespace std;
// use ASCII code 30 it will not show the bar to seperate the delimiter

/**
 * Step 1 : Get the frequency from data
 * Step 2 : then build the Huffman tree
 * Step 3 :  generate the huffmancode base on the trees
 * Step 4 : compress the data using the generated huffman codes 0s and 1s
 * Step 5 : Decompress data using the Huffman Tree
 * Step 6 : Delete the HuffmanTree for memory purposes.
 */
map<char, int> Huffman::frequencyCalculator(std::string& filename) {
    map<char, int> frequencyTable; // Initialize a map to store character frequencies

    ifstream in_file(filename); // Open the input file with the provided filename

    // Check if the file is successfully opened
    if (!in_file.is_open()) {
        cout << "Error opening file.\n"; // Display an error message if the file failed to open
        return frequencyTable; // Return an empty frequency table if file opening fails
    }

    char ch;
    // Read characters from the file one by one until the end of file is reached
    while (in_file.get(ch)) {
        // Calculate the weight of each character by incrementing its frequency in the map
        frequencyTable[ch]++;
    }

    // Close the file as the read operation is complete
    in_file.close();

    return frequencyTable; // Return the map containing character frequencies
}
void Huffman ::HuffmanCodeHelper(Node *root, std::string code, std::map<char, std::string> &codes) {
    //Base Case
    if(root == nullptr){
        return;
    }

    // check if the node is a leaf node
    if(!root->zero && !root->one){
        //Assigns the Generated for this character
        codes[root -> letter] = code;
    }

    //General Case
    //Recursively traverse left and right branches, making them 0 and 1. 0 for left and 1 for right
    HuffmanCodeHelper(root->zero , code +"0", codes);
    HuffmanCodeHelper(root->one , code +"1", codes);
}

void Huffman::genearteHuffmanCode() {
    // Get the root node of the Huffman tree from the priorityQueue
    Node *root = priorityQueue.top();

    // Map to store generated Huffman codes for characters
    std::map<char, std::string> codes;

    // Generate Huffman Codes by calling HuffmanCodeHelper
    HuffmanCodeHelper(root, "", codes);
}
Node * Huffman ::buildHuffmanTree(const map<char,int> &frequencyTable) {
    // Create Huffman nodes for each character-frequency pair and add them to the priority queue
    for (auto i = frequencyTable.begin() ; i!=frequencyTable.end();i++) {
        Node* newNode = new Node(i->first,i->second);
        priorityQueue.push(newNode);
    }

    // Build the Huffman tree by combining nodes until there's only one node left (the root)
    while(priorityQueue.size() > 1){
        // Get the two nodes with the lowest frequencies

        //Get the left side
        Node *zero = priorityQueue.top();
        priorityQueue.pop();

        //Get the right side
        Node *one = priorityQueue.top();
        priorityQueue.pop();

        // Create a new node representing the combined frequencies of the two nodes
        Node *combinedNode = new Node('\0' , zero->weight + one->weight, zero, one);
        combinedNode->zero = zero;
        combinedNode->one = one;

        // Add the combined node back to the priority queue
        priorityQueue.push(combinedNode);
    }

    return priorityQueue.top(); // Return a pointer to the root of the Huffman tree
}

void Huffman ::encode(const std::string &in_File, Storage &compressed_code,
                      const map<char, std::string> &huffmanCodes) {
    ifstream inputFile(in_File);

    if(!inputFile.is_open()){
        cout << "Error opening file " << endl;
        return;
    }
    char c;
    // Read characters from the input file
    while (inputFile.get(c)) {
        // Encode the character using its corresponding Huffman code
        // and insert the encoded bits into the compressed_code Storage
        compressed_code.insert(huffmanCodes.at(c));
    }


    inputFile.close();
}
void Huffman::compress(std::string inputFile,string outputFile) {
    // Create a Storage object to store compressed data
    Storage compressCode;

    // Check if the output file can be opened for writing
    if (!compressCode.open(outputFile, "write")) {
        cout << "Error opening the output file! " << endl;
        return;
    }

    // Calculate the frequency of the data and store it in the weight table
    map<char, int> weightTable = frequencyCalculator(inputFile);

    // Build the Huffman tree using the character frequencies stored in the weight table
    Node* huffmanTree = buildHuffmanTree(weightTable);

    // Create a map to store Huffman codes for each character
    map<char, string> huffmanCodes;

    // Generate Huffman codes for each character using the Huffman tree and store them in the huffmanCodes
    HuffmanCodeHelper(huffmanTree, "", huffmanCodes);

    // Prepare a header for compressed data, combining characters into a single string.
    stringstream buffer;
    for (auto i : weightTable) {
        // Convert the character's frequency to binary representation with a fixed width size.
        // This won't work if the size is too small. it uses
        string binaryRepresentation = bitset<sizeof(int) * 8>(i.second).to_string();
        buffer << i.first << binaryRepresentation << '\30';
    }
    compressCode.setHeader(buffer.str()); // set the prepared header in the compressed code.

    // Encode the input file using Huffman codes and write to the output file in the Storage object(compressCode)
    encode(inputFile, compressCode, huffmanCodes);

    // Delete the Huffman tree to release memory
    deleteHuffmanTree(huffmanTree);

    // Close the output file after writing compressed data
    compressCode.close();
}

Node* Huffman ::createHuffmanTreeFromCompressFile(Storage &compressCode) {
    // This function basically create another tree from the compressed file when decompressing it so that we can rebuild the tree
    // that is stored in the header of the compressed file.
    //  This function will be called in the decompress function.

    // Read the header from the compressed file
    string header;
    header = compressCode.getHeader();

    // Create a map to store letter frequencies
    std::map<char, int> frequencyTable;

    // Parse the header string to extract character frequencies
    char letter;
    int weight;
    int pos = 0;

    while (pos < header.length()) {
        // Find the position of the delimiter '\30'
        int delimiterPos = header.find('\30', pos);
        //from position to the end of the position.
        if (delimiterPos == string::npos) {
            break;  // It has reached the end of the string.
        }

        // This line extracts a segment from the header string starting from the current position up to the position up to position before delimiter.
        string segment = header.substr(pos, delimiterPos - pos);

        // Checks if the segment hast at least two characters.
        if (segment.size() >= 2) {
            // extracting the first character as letter
            letter = segment[0];
            //Retrive the binary weight from the segment string, skipping the first character
            string binaryWeight = segment.substr(1);
            // converts binaryweight to an integer using stoi function
            weight = stoi(binaryWeight, nullptr, 2);
            //update the table with the characters of letter and its weight
            frequencyTable[letter] = weight;
        }

        pos = delimiterPos + 1; // Move the position to the next segment
    }

    // Build a Huffman tree from the extracted frequency table
    Node* huffmanTree = buildHuffmanTree(frequencyTable);
    return huffmanTree;
}

void Huffman::decode(const std::string &out_file, Storage &compressCode, Node*huffmanTree) {
    ofstream outputFile(out_file);
    if(!outputFile.is_open()){
        cout << "Error opening output file from decompression! " << endl;
        return;
    }
    // Set the current node as the root of the Huffman tree
    Node *currNode = huffmanTree;

    // Temporary storage for extracted bits from the compressed data
    string bit;

    // Extract bits from the compressed data and decode them
    while (compressCode.extract(bit) && !bit.empty()) {
        // Iterate through each bit in the extracted string
        for (char b : bit) {
            // Traverse the Huffman tree based on the extracted bits
            if (b == '0') {
                // if it is 0, set the node to the left
                currNode = currNode->zero;
            } else if (b == '1') {
                //else set the node to the right
                currNode = currNode->one;
            }

            // If a leaf node is reached, write the character to the output file
            if (currNode->zero == nullptr && currNode->one == nullptr) {


                outputFile << currNode->letter;
                currNode = huffmanTree; // Reset to the root for the next character
            }
        }
    }
    outputFile.close();
}
void Huffman ::decompress(std::string inputFile,string outputFile) {
    Storage compressCode;

    if(!compressCode.open(inputFile, "read")){
        cout << "Error opening input file for decompression" << endl;
        return;
    }

    // Build the Huffman tree from the compressed file calling the function.
    Node *huffmanTree = createHuffmanTreeFromCompressFile(compressCode);

    // Checking if the Huffman tree was successfully built
    if (huffmanTree == nullptr) {
        cout << "Error building Huffman tree " << endl;
        compressCode.close();
        return;
    }

    // Decode the compressed data using the Huffman tree and write to the output file
    decode(outputFile, compressCode, huffmanTree);

    // Delete the Huffman tree to free memory
    deleteHuffmanTree(huffmanTree);

    // Close the compressed file
    compressCode.close();

}
void Huffman::deleteHuffmanTree(Node* node) {
    //Base case
    if (node == nullptr) {
        return;
    }

    //General case
    //Recursively delete left and right subtrees
    deleteHuffmanTree(node->zero); //Delete the left tree
    deleteHuffmanTree(node->one); // Delete the right tree

    //Delete the current node
    delete node;
}
