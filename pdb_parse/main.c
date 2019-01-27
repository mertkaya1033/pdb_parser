//
//  main.c
//  pdb_parse
//
//  Created by Mert Kaya on 2019-01-25.
//  Copyright © 2019 Mert Kaya. All rights reserved.
//

#include <stdio.h>
#include "pdb_parse.h"

int main(int argc, const char * argv[]) {
    Protein* protein = new_protein("/Users/mert/Desktop/programs/C_Programs/pdb_parse/3nir.pdb");
    printf("Number of atoms: %d\n", protein->num_atoms);
    printf("Number of residues: %d\n", protein->num_residues);
    printf("Number of chains: %d\n", protein->num_chains);
    free_protein(protein);
    return 0;
}
