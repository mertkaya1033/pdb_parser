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
    Protein* protein = new_protein("/Users/mert/Desktop/programs/C/pdb_parser/3nir.pdb");
    
    contacts(protein, 2, 3);
    
    free_protein(protein);
    return 0;
}
