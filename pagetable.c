#include "config.h"
#include "mlpt.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

// deallocate, if ptbr == null or zero, return ~0. check if the current ptbr, loop through all levels, calculating pte and check
//if valid bit , free physical page number. free(physical_page). 

size_t ptbr;
size_t mask = (1<<(POBITS-3)) - 1;


size_t translate(size_t va){ 

    if (!ptbr){
        return ~0;
    }
    //spaces between operators
    size_t offsetMask = ( 1 << POBITS ) - 1;
    size_t offset = va & offsetMask;  
    size_t physicalAddress = -1;
    size_t currentPointer =  ptbr;
    for ( int i = 1 ; i <= LEVELS ; i++ ){
        
        size_t tempVPN = va >> ( POBITS ) >> ( ( LEVELS-i ) *( POBITS-3 ) );
        size_t VPN = tempVPN & mask;
        size_t pageTableEntry = ((size_t *)currentPointer)[VPN];

        if ( (pageTableEntry & 1) == 1 && i != LEVELS){
            pageTableEntry = (((pageTableEntry)>>POBITS)<<POBITS);
            currentPointer = pageTableEntry;
        }
        else if ( ((pageTableEntry & 1 ) == 1) && (i == LEVELS)){
            pageTableEntry = (((pageTableEntry)>>POBITS)<<POBITS);
            physicalAddress = pageTableEntry + offset;
            return physicalAddress;  
        }
        else{
            return ~0;
        }
    
    }
    return physicalAddress; 


}
void page_allocate(size_t va){

    int size = pow(2,POBITS);
    if (ptbr == 0){
        posix_memalign((void**) &ptbr,size,size);
        memset((void*) ptbr , 0 ,size);
    }
    //use a better variable name than cur
    size_t currentPointer = ptbr; 
    for (int i = 1; i<=LEVELS;i++){
        size_t tempsVPN = va>>(POBITS)>>((LEVELS-i)*(POBITS-3)) ;
        size_t VPN = tempsVPN & mask;
        // use underscores or Capitalization in variable names
        size_t pageTableEntry = ((size_t *)currentPointer)[VPN];
        if (( (pageTableEntry) & 1) != 1){
            size_t NewPA;
            posix_memalign((void**) &NewPA,size,size);
            memset((void*) NewPA,0,size);
            NewPA = ((NewPA>>POBITS)<<POBITS) + 1;
            ((size_t*)currentPointer)[VPN] = NewPA;
            NewPA = (NewPA-1); 
            currentPointer = NewPA;
        }
        else {
            // you need spaces between operators
            currentPointer = ( ( pageTableEntry ) >> POBITS ) << POBITS;
        }
        
    
    }                      

}


    



