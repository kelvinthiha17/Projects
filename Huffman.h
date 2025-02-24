#include <string>
#include <queue>
#include <fstream>
#include "Node.h"
#include "Storage/Storage.h"
#include <map>
using namespace std;

#ifndef HUFFMAN_H
#define HUFFMAN_H

class Huffman {
public:
    /**
     * Takes an inputFile and compress it
     * @param inputFile
     * @param outputFile
     */
    void compress (string inputFile,string outputFile);
    /**
     * Takes in an inputfile and then decompress it
     * @param inputFile
     * @param outputFile
     */
    void decompress(string inputFile,string outputFile);

private:

    /**
     * Standard libary priority queue
     */
    std::priority_queue<Node*, std::vector<Node*>, compareWeights> priorityQueue;
    /**
     * This function is to calculate the Frequency of the data
     * @param filename
     * @return
     */
    map<char, int> frequencyCalculator(string& filename);
    /**
     * Building the Huffman Tree
     */
    Node * buildHuffmanTree(const map<char,int> &table);
    /**
     * Helper function
     * @param root
     * @param code
     * @param codes
     */
    void HuffmanCodeHelper(Node* root, string code, map<char, std::string>& codes);

    /**
     * This function is to generate Huffman codes
     */
    void genearteHuffmanCode();

    /**
     * Encoding the file using huffman codes and write it to the input File
     * @param in_File
     * @param compressed_code
     * @param huffmanCodes
     */
    void encode(const string& in_File,Storage& compressed_code,const map<char, string> &huffmanCodes);

    /**
     * create another tree from the compressed file
     * @param compressCode
     * @return
     */
    Node *createHuffmanTreeFromCompressFile(Storage & compressCode);

    /**
     * Takes in inputfile and then decompress it and writes the compressed data to an outputFile
     * @param out_file
     * @param compress_code
     * @param huffmanTree
     */
    void decode(const string& out_file, Storage &compressCode , Node * huffmanTree);

    /**
     * Delete Huffman Tree function that delete the whole huffmanTree to save up memories in the computer
     * @param node
     */
    void deleteHuffmanTree(Node * node);


};
#endif //HUFFMAN_H
