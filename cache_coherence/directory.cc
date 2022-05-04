/************************************************************
                        Course          :       CSC506
                        Source          :       directory.cc
                        Owner           :       Ed Gehringer
                        Email Id        :       efg@ncsu.edu
------------------------------------------------------------*
        © Please do not replicate this code without consulting
                the owner & instructor! Thanks!
*************************************************************/
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
using namespace std;

#include "directory.h"
#include "main.h"
#include "fbv.h"
#include "ssci.h"

// Kindly don't edit this file as of now. This is a part of Program 4



ulong Directory::dir_size = 0;

Directory::Directory(ulong num_entries, int type) {
}

Directory::~Directory() {
}

dir_entry *Directory::find_dir_line(ulong tag) {
        return NULL;
}

dir_entry *Directory::find_empty_line(ulong tag) {
	return NULL;
}

