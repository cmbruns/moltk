#include "moltk/pdb.h"

using namespace std;
using namespace moltk;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "File name argument missing" << endl;
        return 1; // file name argument missing
    }
    std::string pdbFileName(argv[1]);

    pdb::Structure pdbStructure(pdbFileName);

    return 0;
}

