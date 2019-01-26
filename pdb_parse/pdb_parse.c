//
//  pdb_parse.c
//  pdb_parse
//
//  Created by Mert Kaya on 2019-01-25.
//  Copyright © 2019 Mert Kaya. All rights reserved.
//

#include "pdb_parse.h"

Protein* new_protein(char *file_name){
    Protein* newP = (Protein*) malloc(sizeof(Protein));
    newP->num_atoms = 0;
    newP->num_residues = 0;
    newP->num_chains = 0;
    newP->atoms = (Atom*) malloc(100000 * sizeof(Atom));
    newP->residues =(Residue*) malloc(70000 * sizeof(Residue));
    newP->chains = (Chain*) malloc(100 * sizeof(Chain));
    
    int current_residueID;
    char current_chainID;
    
    FILE* file = fopen(file_name, "r");
    while(!feof(file)){
        
    }
    
    return newP;
};

