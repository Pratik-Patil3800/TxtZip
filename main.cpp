# include<iostream>
#include<queue>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <bitset>
#include <cstring>
using namespace std;

class node{
    public:
    char data;
    int freq;
    node* left;
    node* right;
    node(char data,int freq){
        this->data=data;
        this->freq=freq;
        this->left=NULL;
        this->right=NULL;
    }
};

struct CompareNodes {
    bool operator()(const node* a, const node* b) const {
        return a->freq > b->freq;
    }
};

string header(const unordered_map<char, string>& codes, int offset) {
    string headerStr="`";
    for (const auto& pair : codes) {
        headerStr += pair.first; 
        headerStr +=pair.second + "`"; 
    }
    headerStr += to_string(offset);
    return headerStr;
}

node* hofmantree(unordered_map<char, int>& freqMap){
    priority_queue<node*,vector<node*>, CompareNodes> pq;
    for (auto& pair : freqMap) {
        pq.push(new node(pair.first, pair.second));
    }
    
    while (pq.size()>1) {
        node* left = pq.top(); pq.pop();
        node* right = pq.top(); pq.pop();
        node* parent = new node('$', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    return pq.top();
}

void generateHuffmanCodes(node* root, string code, unordered_map<char, string>& codes) {
    if (root == nullptr) return;

    if (root->data != '$') {
        codes[root->data] = code;
    }
    generateHuffmanCodes(root->left, code + "0", codes);
    generateHuffmanCodes(root->right, code + "1", codes);
}



void writeEncodedData(const string& filecontent, const unordered_map<char, string>& codes, const string& outputFileName) {
    ofstream outputFile(outputFileName, ios::binary);
    if (outputFile.is_open()) {
       
        unsigned char buffer = 0; 
        int bitsInBuffer = 0; 
        
        for (char c : filecontent) {
            for (char bit : codes.at(c)) {
                buffer = (buffer << 1) | (bit - '0');
                bitsInBuffer++;
                
                if (bitsInBuffer == 8) {
                    outputFile.put(buffer);
                    buffer = 0;
                    bitsInBuffer = 0;
                }
            }
        }
        int offset=bitsInBuffer;
        if (bitsInBuffer > 0) {
            buffer <<= (8 - bitsInBuffer);
            outputFile.put(buffer);
        }

        string codesHeader = header(codes, offset);
        for (char c : codesHeader) {
            outputFile.put(c);
        }
        
        outputFile.close();
        cout << "Compressed data written to " << outputFileName << endl;
    } else {
        cerr << "Error: Unable to open output file for writeEncodedData " << outputFileName << endl;
    }
}

string readBinaryDataFromFile(const string& filename) {
    ifstream file(filename, ios::binary);
    string binaryString;
    if (file.is_open()) {
        file.seekg(0, ios::end);
        streampos fileSize = file.tellg();
        file.seekg(0, ios::beg);
        binaryString.resize(fileSize);
        file.read(&binaryString[0], fileSize);
        file.close();
    } else {
        cerr << "Error: Unable to open file for readBinaryDataFromFile" << filename << endl;
    }
    return binaryString;
}

string* reverseencoding(const string& filename){
    string* filecontent=new string (readBinaryDataFromFile(filename));
    int i=0;
    string* encodin=new string("");
    while(i<filecontent->size()){
        if((*filecontent)[i]=='`'){
            i++;
            break;
        }
        encodin->push_back((*filecontent)[i]);
        i++;
    }
    unordered_map<string,char>* mp=new unordered_map<string,char>();
    string* temp=new string("");
    while(i<filecontent->size()){
        if((*filecontent)[i]=='`'){
            (*mp)[temp->substr(1)]=(*temp)[0];
            temp->clear();
        }
        else{
            temp->push_back((*filecontent)[i]);
        }
        i++;
    }
    int offset=8-stoi(*temp);
    string binaryString;
    for (char c : *encodin) {
        binaryString += bitset<8>(c).to_string();
    }
    string* ans=new string("");
    string* findkey=new string("");
    for(int i=0;i<binaryString.length()-offset;i++){
        findkey->push_back(binaryString[i]);
        if(mp->find(*findkey)!=mp->end()){
            ans->push_back((*mp)[*findkey]);
            findkey->clear();
        }
    }
    delete filecontent;
    delete encodin;
    delete mp;
    delete temp;
    // delete binaryString;
    delete findkey;
    return ans;
}

void compress(const string& filename){
    ifstream inputFile(filename, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
    }
    else{
        // unordered_map<char, int> freqMap;
        string encodedText;
        string text((istreambuf_iterator<char>(inputFile)), (istreambuf_iterator<char>()));
        inputFile.close();

        unordered_map<char, int> freqMap;
        for (char c : text) {
            freqMap[c]++;
        }
        node* root=hofmantree(freqMap);
        unordered_map<char, string> codes;
        generateHuffmanCodes(root,"",codes);
        // encodedText = encodeText(text, codes);
        string outputfilename="";
        for(int i=0;i<filename.size();i++){
            if(filename[i]=='.'){
                break;
            }
            outputfilename.push_back(filename[i]);
        }
        outputfilename+=".bin";
        writeEncodedData(text, codes, outputfilename);
    } 
}

void decompress(const string& filename){
    string* ans=reverseencoding(filename);
    string outputfilename="";
    for(int i=0;i<filename.size();i++){
        if(filename[i]=='.'){
            break;
        }
        outputfilename.push_back(filename[i]);
    }
    outputfilename+=".txt";
    
    ofstream outputFile(outputfilename);
    if (outputFile.is_open()) {
        outputFile << ans;
        outputFile.close();
    } else {
        cerr << "Error: Unable to open output file for writeEncodedData " << outputfilename << endl;
    }
}

int main(int argc,char** argv){
    cout<<argv[1]<<" "<<argv[2]<<endl;
     if (strcmp(argv[1], "compress") == 0) {
        compress(argv[2]);
    } else {
        decompress(argv[2]);
    }
    return 0;
}