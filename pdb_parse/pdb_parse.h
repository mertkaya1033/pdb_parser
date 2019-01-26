//
//  pdb_parse.h
//  pdb_parse
//
//  Created by Mert Kaya on 2019-01-25.
//  Copyright © 2019 Mert Kaya. All rights reserved.
//

#ifndef pdb_parse_h
#define pdb_parse_h

#include <stdio.h>
#include <stdlib.h>

typedef struct Atom{
    int ID;
    char name[5];
    double coor[3];
    struct Residue* res;
} Atom;

typedef struct Residue{
    int ID;
    char name[4];
    Atom* atoms;
    int num_atoms;
    struct Chain* chains;
} Residue;

typedef struct Chain{
    char ID;
    Residue* residues;
    int num_residues;
    struct Protein* protein;
} Chain;

typedef struct Protein{
    char* name;
    Atom* atoms;
    Residue* residues;
    Chain* chains;
    int num_atoms;
    int num_residues;
    int num_chains;
} Protein;

Protein* new_protein(char* file_name);
void free_protein(Protein* p);
#endif /* pdb_parse_h */
