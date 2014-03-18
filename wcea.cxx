#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <tktools.hxx>

using namespace std;

using namespace tktools;
using namespace tktools::io;
using namespace tktools::util;
using namespace tktools::bio;

namespace {
    void show_help() {
        cerr << "wcea <options>\n";
        cerr << "OPTIONS " << endl;
        cerr << "  -1 <filename>     : <sorted bam file>" << endl;
        cerr << "  -2 <filename>     : <sorted bam file>" << endl;
        cerr << "  -o <filename>     : output file\n" << endl;
        cerr << "  -w <integer>      : window size (default:100)" << endl;
        cerr << "  -s <integer>      : step (default:100)" << endl;
        cerr << "  -t <float>        : detection threshold off odds ratio (2.0)" << endl;
        cerr << "  -verbose           verbose mode" << endl;
    }
}

int main(int argc, char** argv) {
    try {
        //const char* filename = get_argument_string(argc, argv, "i", NULL);
        const char* filename_output = get_argument_string(argc, argv, "o", NULL);
        //bool verbose = has_option(argc, argv, "verbose");

        const char* filename_bam1 = get_argument_string(argc, argv, "1", NULL);
        const char* filename_bam2 = get_argument_string(argc, argv, "2", NULL);
        const char* filename_output = get_argument_string(argc, argv, "o", NULL);
        int window_size = get_argument_integer(argc, argv, "w", 100);
        int step = get_argument_integer(argc, argv, "s", 100);
        double threshold = get_argument_float(argc, argv, "t", 2.0);
        bool verbose = has_option(argc, argv, "verbose");

        bamFile bamfile1;
        bamFile bamfile2;
        bam1_t* read1;
        bam1_t* read2;
        bam_header_t* header1;
        bam_header_t* header2;

        if ((window_size / step) * step != window_size) {
            throw invalid_argument("window size must be divided by step");
        }
        if (window_size < 1 || step < 1) {
            throw invalid_argument("window size and step must be positive number");
        }
        if (filename_bam1 == NULL || file_exists(filename_bam1) == false
            || filename_bam2 == NULL || file_exists(filename_bam2) == false) {
        }

        if (verbose) {
        }

        ostream* ost = &cout;
        if (filename_output != NULL) {
            ost = new ofstream(filename_output);
            if (ost->is_open() == false) {
                throw invalid_argument("cannot open output stream");
            }
        }

        bamfile1 = bam_open(filename_bam1, "rb");
        bamfile2 = bam_open(filename_bam2, "rb");
        read1 = bam_init1();
        read2 = bam_init2();
        int mode = 3; // 1 : read 1 , 2: read 2, 3 : read both
        int total1 = 0;
        int total2 = 0;
        int binstart = 0;
        int binend = binstart + window_size;
        int current_chromosome = -1;
        for (;;) {
            if ((mode & 1) != 0) {
                if (bam_read1(bamfile1, read1) <= 0) break;
            }
            if ((mode & 2) != 0) {
                if (bam_read2(bamfile2, read2) <= 0) break;
            }
            // chromosome
            int c1 = read1->core.tid;
            int c2 = read2->core.tid;
            // unmapped
            if (c1 < 0) {
                if (c2 < 0) {
                    mode = 3;
                    continue;
                } else {
                    mode = 1;
                    continue;
                }
            } else if (c2 < 0) {
                mode = 2;
                continue;
            }
            // different chromosome
            if (c1 < c2) {
                mode = 1;
                display_cnv(total_1, total_2, 
                continue;
            } else if (c1 > c2) {
                mode = 2;
                continue;
            }

            // position
            int pos1 = read1->core.pos;
            int pos2 = read2->core.pos;
            int len1 = read1->core.l_qseq;
            int len2 = read2->core.l_qseq;
            
        }
        
        bam_destroy1(read1);
        bam_destroy1(read2);
        bam_header_destroy(header1);
        bam_header_destroy(header2);
        bam_close(bamfile1);
        bam_close(bamfile2);


        if (filename_output != NULL) {
            dynamic_cast<ofstream*>(ost)->close();
            delete ost;
        }
        return 0;
    } catch (exception& e) {
        cerr << e.what() << endl;
        return -1;
    }
}

