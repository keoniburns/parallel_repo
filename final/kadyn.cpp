#include <sndfile.h>

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <vector>
using std::cout;
using std::vector;
namespace fs = std::filesystem;

static vector<float> ReadFile(const char* fname);
static void CreateFile(const char* fname, int format, vector<float> dat);

static void CreateFile(const char* fname, int format, vector<float> dat) {
    SndfileHandle file;
    int channels = 2;
    int srate = 44100;
    if (fs::exists(fname)) {
        fs::remove(fname);
        cout << "Overwriting file." << std::endl;
    }
    printf("Creating file named '%s'\n", fname);
    file = SndfileHandle(fname, SFM_WRITE, format, channels, srate);
    file.write(&dat[0], dat.size() * 2);
    ReadFile(fname);
    puts("");  // ?
}

static vector<float> ReadFile(const char* fname) {
    SndfileHandle file;
    file = SndfileHandle(fname);
    printf("Opened file '%s'\n", fname);
    printf("  sample rate: %d\n", file.samplerate());
    printf("  channels: %d\n", file.channels());
    printf("  frames: %ld\n", file.frames());
    vector<float> array(file.frames());
    file.read(&array[0], int(file.frames()));
    printf("Array size: %ld\n", array.size());
    puts("");
    return array;
}

// Add wavefrom transforms here.
void EditFrames(vector<float>& val) {
    // for (int i = val.size() - 1; i < val.size(); i++) {
    // // Edit here
    // }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Use: ./PROGRAM [infilename] [outfilename]" << std::endl;
        exit(1);
    }

    std::string fname = argv[1];
    vector<float> fromarr = ReadFile(fname.c_str());

    EditFrames(fromarr);

    CreateFile(argv[2], SF_FORMAT_WAV | SF_FORMAT_PCM_16, fromarr);
    puts("Done\n");
    return 0;
}

void makeWindows(double frameSize, ) {}