#include <iostream>
#include <fstream>
#include <sys/stat.h>

using namespace std;

class WavHeader{
public:
    // RIFF Descriptor
    uint8_t  RIFF[4];
    uint32_t fileSize;
    uint8_t WAVE[4];

    // fmt sub-chunk
    uint8_t fmt[4];
    uint32_t fmtSize;
    uint16_t audioFormat;
    uint16_t noChannels;
    uint32_t sampleRate;
    uint32_t bytesPerSec;
    uint16_t blockAlign;
    uint16_t bitsPerSec;

    // data sub-chunk
    uint8_t data[4];
    uint32_t dataSize;

    WavHeader(uint32_t fileSize, uint16_t noChannels, uint32_t sampleRate,
            uint16_t bitsPerSec, uint32_t dataSize){

        // RIFF descriptor
        RIFF[0] = 'R';RIFF[1] = 'I';RIFF[2] = 'F';RIFF[3] = 'F';
        this->fileSize = fileSize;
        WAVE[0] = 'W';WAVE[1] = 'A';WAVE[2] = 'V';WAVE[3] = 'E';

        // fmt sub-chunk
        fmt[0] = 'f';fmt[1] = 'm';fmt[2] = 't';fmt[3] = 32;
        fmtSize = 16;
        audioFormat = 1;
        this->noChannels = noChannels;
        this->sampleRate = sampleRate;
        this->bitsPerSec = bitsPerSec;
        blockAlign = (bitsPerSec * noChannels) / 8;
        bytesPerSec = (sampleRate * bitsPerSec * noChannels) / 8;

        // data sub-chunk
        data[0] = 'd';data[1] = 'a';data[2] = 't';data[3] = 'a';
        this->dataSize = dataSize;

    }

    WavHeader(){}

};


string getFileBaseName(string filePath);
string getOutputFileName(string fileName);
void takeInput(string& key, string& filePath);
void hide(string key, string filePath, int channels = 2, int sampleRate = 41000, int bitsPerSec = 16);
void seek(string key, string filePath);


int main(){

    int choice = 1;
    string key, filePath;
    while( choice != 3 ){

        cout <<"1. Hide\n2. Seek\n3. Exit\n";
        cout <<"Enter choice = ";
        cin >> choice;
        cin.ignore();

        switch(choice){
            case 1:
                takeInput(key, filePath);
                hide(key, filePath);
                break;
            case 2:
                takeInput(key, filePath);
                seek(key, filePath);
                break;
            case 3:
                break;
            default:
                cout <<"\n*** Invalid Choice ***";
                break;
        }

    }

    cout << endl;
    return 0;

}


void hide(string key, string filePath, int channels, int sampleRate, int bitsPerSec){

    /*
     * Create a .wav file & hide
     * the filePath file in it
    */

    // get current date-time to use it as output file name
    time_t currentTime;
    time(&currentTime);
    string curTimeStr = asctime(localtime(&currentTime));
    curTimeStr.erase(curTimeStr.end() - 1);   // remove trailing '\n'

    clock_t start, end;
    start = clock();
    cout << "\nHiding File..\r";
    cout.flush();

    ifstream inFile(filePath, ios::binary);
    // get unique file name to avoid overwriting any other file
    string outputFileName = getOutputFileName(curTimeStr + ".wav");
    ofstream outFile(outputFileName, ios::binary);

    int headerSize = 16;
    int bytesPerFrame = channels * (bitsPerSec / 8);

    // get file size
    inFile.seekg(0, ios_base::end);
    uint32_t fileSize = inFile.tellg();
    inFile.seekg(0, ios_base::beg);

    // get file name from filePath
    string fileName = getFileBaseName(filePath);

    // seed random generator with the hash of the input key
    srand(hash<string>{}(key));

    // XOR filename before storing it in the wav file
    for(int i = 0; i < fileName.size(); i++){
        fileName[i] ^= (random() % 256);
    }

    // wav header
    WavHeader header = WavHeader(fileSize + headerSize, channels, sampleRate,
        bitsPerSec, fileSize);


    // write wav header
    outFile.write((char *)&header, sizeof(header));

    // store length of file name and the file name in the wav file
    // when the original file is to be retrieved from the wav file, this
    // stored fileName is used as the output file name
    uint16_t filenameSize = fileName.size();
    outFile.write((char *) &filenameSize, sizeof(filenameSize));
    outFile.write(fileName.c_str(), filenameSize);


    // iterate through the input file and write to generated wav file
    // the source data is XORed with some random value before writting
    // to the wav file
    uint8_t data;
    while( ! inFile.eof() ){
        inFile.read((char *)&data, sizeof(data));
        data ^= (random() % 256);
        outFile.write((char *)&data, sizeof(data));
    }

    // pad data if required
    // ideally fileSize % bytesPerFrame should be 0
    // otherwise, we need to add some extra bytes to fill the frame
    fileSize = fileSize % bytesPerFrame;
    data = 0;
    while(fileSize > 0){
        outFile.write((char *) &data, sizeof(data));
        fileSize--;
    }

    outFile.close();
    inFile.close();

    end = clock();
    cout <<"File Saved as : " <<outputFileName;
    cout <<"\nCompleted in " <<((float)(end - start)) / CLOCKS_PER_SEC <<" seconds\n\n";

}


void seek(string key, string filePath){

    /*
     * extracting the file hidden in the
     * wav file
    */

    clock_t start, end;
    start = clock();
    cout << "\nSeeking File..\r";
    cout.flush();

    ifstream inFile(filePath, ios::binary);

    // read wav header
    WavHeader header;
    inFile.read((char *) &header, sizeof(header));

    // read the length of the source (hidden) file name
    uint16_t filenameSize;
    inFile.read((char *) &filenameSize, sizeof(filenameSize));

    // read source (hidden) file name
    char fileName[filenameSize + 1];
    inFile.read(fileName, filenameSize);
    fileName[filenameSize] = '\0';

    // seed random generator with the hash of the input key
    srand(hash<string>{}(key));

    // XOR the file name read from wav file to get the actual file name
    for(int i = 0; i < filenameSize; i++){
        fileName[i] ^= (random() % 256);
    }

    // get unique file name to avoid overwriting any other file
    string outputFileName = getOutputFileName(string(fileName));

    // get size of source (hidden) file
    uint32_t fileSize = header.dataSize;

    ofstream outFile(outputFileName, ios::binary);

    // read data from wav file & write to the output file
    // data read from wav file is XORed with some random value
    // to get the actual data
    uint8_t data;
    while( fileSize && ! inFile.eof() ){
        inFile.read((char *)&data, sizeof(data));
        data ^= (random() % 256);
        outFile.write((char *)&data, sizeof(data));
        fileSize--;
    }

    outFile.close();
    inFile.close();

    end = clock();
    cout <<"File Saved as : " <<outputFileName;
    cout <<"\nCompleted in " <<((float)(end - start)) / CLOCKS_PER_SEC <<" seconds\n\n";

}


string getFileBaseName(string filePath){

    /*
     * given an absolute / relative path,
     * return only the file name
    */

    int lastPos = filePath.find_last_of("/") + 1;
    filePath.erase(filePath.begin(), filePath.begin() + lastPos);
    return filePath;

}


string getOutputFileName(string fileName){

    /*
     * find unique file name so we dont overwrite
     * any existing files
    */

    // if no such file exists, return the given file name
    struct stat s;
    if( stat(fileName.c_str(), &s) != 0 )
        return fileName;


    // split file name & extension
    string name, extension;

    int lastPoint = fileName.find_last_of(".");
    if( lastPoint == -1 ){
        name = fileName;
        extension = "";
    }
    else {
        name = string(fileName.begin(), fileName.begin() + lastPoint);
        extension = string(fileName.begin() + lastPoint, fileName.end());
    }

    // append counter to file name until we get a unique file name
    int counter = 0;
    while( stat(fileName.c_str(), &s) != 0 ){
        counter++;
        fileName = name + " - " + to_string(counter) + extension;
    }

    return fileName;

}


void takeInput(string& key, string& filePath){

    char k[20];
    cout <<"\nEnter key = ";
    cin.getline(k, 50);
    key = string(k);

    char fP[200];
    while( true ){
        cout <<"Enter file path = ";
        cin.getline(fP, 200);

        // check if input file path is valid
        struct stat s;
        if( stat(fP, &s) == 0 ){
            if( s.st_mode & S_IFDIR ){
                cout <<"\n *** Input path is a directory, input file path *** \n\n";
                continue;
            }
        }
        else {
            cout <<"\n *** No Such File Exists *** \n\n";
            continue;
        }

        break;

    }
    filePath = string(fP);

}
