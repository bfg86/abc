/**CFile****************************************************************

  FileName    [extraUtilMisc.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [extra]

  Synopsis    [Misc procedures.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: extraUtilMisc.c,v 1.0 2003/09/01 00:00:00 alanmi Exp $]

***********************************************************************/

#include "extra.h"

/*---------------------------------------------------------------------------*/
/* Constant declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Stucture declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Type declarations                                                         */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Variable declarations                                                     */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Macro declarations                                                        */
/*---------------------------------------------------------------------------*/


/**AutomaticStart*************************************************************/

/*---------------------------------------------------------------------------*/
/* Static function prototypes                                                */
/*---------------------------------------------------------------------------*/

/**AutomaticEnd***************************************************************/

static void Extra_Permutations_rec( char ** pRes, int nFact, int n, char Array[] );

/*---------------------------------------------------------------------------*/
/* Definition of exported functions                                          */
/*---------------------------------------------------------------------------*/


/**Function********************************************************************

  Synopsis    [Finds the smallest integer larger of equal than the logarithm.]

  Description [Returns [Log2(Num)].]

  SideEffects []

  SeeAlso     []

******************************************************************************/
int Extra_Base2Log( unsigned Num )
{
    int Res;
    assert( Num >= 0 );
    if ( Num == 0 ) return 0;
    if ( Num == 1 ) return 1;
    for ( Res = 0, Num--; Num; Num >>= 1, Res++ );
    return Res;
} /* end of Extra_Base2Log */

/**Function********************************************************************

  Synopsis    [Finds the smallest integer larger of equal than the logarithm.]

  Description []

  SideEffects []

  SeeAlso     []

******************************************************************************/
int Extra_Base2LogDouble( double Num )
{
    double Res;
    int ResInt;

    Res    = log(Num)/log(2.0);
    ResInt = (int)Res;
    if ( ResInt == Res )
        return ResInt;
    else 
        return ResInt+1;
}

/**Function********************************************************************

  Synopsis    [Finds the smallest integer larger of equal than the logarithm.]

  Description [Returns [Log10(Num)].]

  SideEffects []

  SeeAlso     []

******************************************************************************/
int Extra_Base10Log( unsigned Num )
{
    int Res;
    assert( Num >= 0 );
    if ( Num == 0 ) return 0;
    if ( Num == 1 ) return 1;
    for ( Res = 0, Num--;  Num;  Num /= 10,  Res++ );
    return Res;
} /* end of Extra_Base2Log */

/**Function********************************************************************

  Synopsis    [Returns the power of two as a double.]

  Description []

  SideEffects []

  SeeAlso     []

******************************************************************************/
double Extra_Power2( int Degree )
{
    double Res;
    assert( Degree >= 0 );
    if ( Degree < 32 )
        return (double)(01<<Degree); 
    for ( Res = 1.0; Degree; Res *= 2.0, Degree-- );
    return Res;
}


/**Function*************************************************************

  Synopsis    []

  Description []

  SideEffects []

  SeeAlso     []

***********************************************************************/
int Extra_Power3( int Num )
{
    int i;
    int Res;
    Res = 1;
    for ( i = 0; i < Num; i++ )
        Res *= 3;
    return Res;
}

/**Function********************************************************************

  Synopsis    [Finds the number of combinations of k elements out of n.]

  Description []

  SideEffects []

  SeeAlso     []

******************************************************************************/
int Extra_NumCombinations( int k, int n )
{
    int i, Res = 1;
    for ( i = 1; i <= k; i++ )
            Res = Res * (n-i+1) / i;
    return Res;
} /* end of Extra_NumCombinations */

/**Function*************************************************************

  Synopsis    []

  Description []

  SideEffects []

  SeeAlso     []

***********************************************************************/
int * Extra_DeriveRadixCode( int Number, int Radix, int nDigits )
{
    static int Code[100];
    int i;
    assert( nDigits < 100 );
    for ( i = 0; i < nDigits; i++ )
    {
        Code[i] = Number % Radix;
        Number  = Number / Radix;
    }
    assert( Number == 0 );
    return Code;
}

/**Function********************************************************************

  Synopsis    [Computes the factorial.]

  Description []

  SideEffects []

  SeeAlso     []

******************************************************************************/
int Extra_Factorial( int n )
{
    int i, Res = 1;
    for ( i = 1; i <= n; i++ )
        Res *= i;
    return Res;
}

/**Function********************************************************************

  Synopsis    [Computes the set of all permutations.]

  Description [The number of permutations in the array is n!. The number of
  entries in each permutation is n. Therefore, the resulting array is a 
  two-dimentional array of the size: n! x n. To free the resulting array,
  call free() on the pointer returned by this procedure.]

  SideEffects []

  SeeAlso     []

******************************************************************************/
char ** Extra_Permutations( int n )
{
    char Array[50];
    char ** pRes;
    int nFact, i;
    // allocate memory
    nFact = Extra_Factorial( n );
    pRes  = (char **)Extra_ArrayAlloc( nFact, n, sizeof(char) );
    // fill in the permutations
    for ( i = 0; i < n; i++ )
        Array[i] = i;
    Extra_Permutations_rec( pRes, nFact, n, Array );
    // print the permutations
/*
    {
    int i, k;
    for ( i = 0; i < nFact; i++ )
    {
        printf( "{" );
        for ( k = 0; k < n; k++ )
            printf( " %d", pRes[i][k] );
        printf( " }\n" );
    }
    }
*/
    return pRes;
}

/**Function********************************************************************

  Synopsis    [Fills in the array of permutations.]

  Description []

  SideEffects []

  SeeAlso     []

******************************************************************************/
void Extra_Permutations_rec( char ** pRes, int nFact, int n, char Array[] )
{
    char ** pNext;
    int nFactNext;
    int iTemp, iCur, iLast, k;

    if ( n == 1 )
    {
        pRes[0][0] = Array[0];
        return;
    }

    // get the next factorial
    nFactNext = nFact / n;
    // get the last entry
    iLast = n - 1;

    for ( iCur = 0; iCur < n; iCur++ )
    {
        // swap Cur and Last
        iTemp        = Array[iCur];
        Array[iCur]  = Array[iLast];
        Array[iLast] = iTemp;

        // get the pointer to the current section
        pNext = pRes + (n - 1 - iCur) * nFactNext;

        // set the last entry 
        for ( k = 0; k < nFactNext; k++ )
            pNext[k][iLast] = Array[iLast];

        // call recursively for this part
        Extra_Permutations_rec( pNext, nFactNext, n - 1, Array );

        // swap them back
        iTemp        = Array[iCur];
        Array[iCur]  = Array[iLast];
        Array[iLast] = iTemp;
    }
}

/**Function*************************************************************

  Synopsis    [Permutes the given vector of minterms.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Extra_TruthPermute_int( int * pMints, int nMints, char * pPerm, int nVars, int * pMintsP )
{
    int m, v;
    // clean the storage for minterms
    memset( pMintsP, 0, sizeof(int) * nMints );
    // go through minterms and add the variables
    for ( m = 0; m < nMints; m++ )
        for ( v = 0; v < nVars; v++ )
            if ( pMints[m] & (1 << v) )
                pMintsP[m] |= (1 << pPerm[v]);
}

/**Function*************************************************************

  Synopsis    [Permutes the function.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned Extra_TruthPermute( unsigned Truth, char * pPerms, int nVars, int fReverse )
{
    unsigned Result;
    int * pMints;
    int * pMintsP;
    int nMints;
    int i, m;

    assert( nVars < 6 );
    nMints  = (1 << nVars);
    pMints  = ALLOC( int, nMints );
    pMintsP = ALLOC( int, nMints );
    for ( i = 0; i < nMints; i++ )
        pMints[i] = i;

    Extra_TruthPermute_int( pMints, nMints, pPerms, nVars, pMintsP );

    Result = 0;
    if ( fReverse )
    {
        for ( m = 0; m < nMints; m++ )
            if ( Truth & (1 << pMintsP[m]) )
                Result |= (1 << m);
    }
    else
    {
        for ( m = 0; m < nMints; m++ )
            if ( Truth & (1 << m) )
                Result |= (1 << pMintsP[m]);
    }

    free( pMints );
    free( pMintsP );

    return Result;
}

/**Function*************************************************************

  Synopsis    [Changes the phase of the function.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned Extra_TruthPolarize( unsigned uTruth, int Polarity, int nVars )
{
    // elementary truth tables
    static unsigned Signs[5] = {
        0xAAAAAAAA,    // 1010 1010 1010 1010 1010 1010 1010 1010
        0xCCCCCCCC,    // 1010 1010 1010 1010 1010 1010 1010 1010
        0xF0F0F0F0,    // 1111 0000 1111 0000 1111 0000 1111 0000
        0xFF00FF00,    // 1111 1111 0000 0000 1111 1111 0000 0000
        0xFFFF0000     // 1111 1111 1111 1111 0000 0000 0000 0000
    };
    unsigned uTruthRes, uCof0, uCof1;
    int nMints, Shift, v;
    assert( nVars < 6 );
    nMints = (1 << nVars);
    uTruthRes = uTruth;
    for ( v = 0; v < nVars; v++ )
        if ( Polarity & (1 << v) )
        {
            uCof0  = uTruth & ~Signs[v];
            uCof1  = uTruth &  Signs[v];
            Shift  = (1 << v);
            uCof0 <<= Shift;
            uCof1 >>= Shift;
            uTruth = uCof0 | uCof1;
        }
    return uTruth;
}

/**Function*************************************************************

  Synopsis    [Computes N-canonical form using brute-force methods.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned Extra_TruthCanonN( unsigned uTruth, int nVars )
{
    unsigned uTruthMin, uPhase;
    int nMints, i;
    nMints    = (1 << nVars);
    uTruthMin = 0xFFFFFFFF;
    for ( i = 0; i < nMints; i++ )
    {
        uPhase = Extra_TruthPolarize( uTruth, i, nVars ); 
        if ( uTruthMin > uPhase )
            uTruthMin = uPhase;
    }
    return uTruthMin;
}

/**Function*************************************************************

  Synopsis    [Computes NN-canonical form using brute-force methods.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned Extra_TruthCanonNN( unsigned uTruth, int nVars )
{
    unsigned uTruthMin, uTruthC, uPhase;
    int nMints, i;
    nMints    = (1 << nVars);
    uTruthC   = (unsigned)( (~uTruth) & ((~((unsigned)0)) >> (32-nMints)) );
    uTruthMin = 0xFFFFFFFF;
    for ( i = 0; i < nMints; i++ )
    {
        uPhase = Extra_TruthPolarize( uTruth, i, nVars ); 
        if ( uTruthMin > uPhase )
            uTruthMin = uPhase;
        uPhase = Extra_TruthPolarize( uTruthC, i, nVars ); 
        if ( uTruthMin > uPhase )
            uTruthMin = uPhase;
    }
    return uTruthMin;
}

/**Function*************************************************************

  Synopsis    [Computes P-canonical form using brute-force methods.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned Extra_TruthCanonP( unsigned uTruth, int nVars )
{
    static int nVarsOld, nPerms;
    static char ** pPerms = NULL;

    unsigned uTruthMin, uPerm;
    int k;

    if ( pPerms == NULL )
    {
        nPerms = Extra_Factorial( nVars );   
        pPerms = Extra_Permutations( nVars );
        nVarsOld = nVars;
    }
    else if ( nVarsOld != nVars )
    {
        free( pPerms );
        nPerms = Extra_Factorial( nVars );   
        pPerms = Extra_Permutations( nVars );
        nVarsOld = nVars;
    }

    uTruthMin = 0xFFFFFFFF;
    for ( k = 0; k < nPerms; k++ )
    {
        uPerm = Extra_TruthPermute( uTruth, pPerms[k], nVars, 0 );
        if ( uTruthMin > uPerm )
            uTruthMin = uPerm;
    }
    return uTruthMin;
}

/**Function*************************************************************

  Synopsis    [Computes NP-canonical form using brute-force methods.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned Extra_TruthCanonNP( unsigned uTruth, int nVars )
{
    static int nVarsOld, nPerms;
    static char ** pPerms = NULL;

    unsigned uTruthMin, uPhase, uPerm;
    int nMints, k, i;

    if ( pPerms == NULL )
    {
        nPerms = Extra_Factorial( nVars );   
        pPerms = Extra_Permutations( nVars );
        nVarsOld = nVars;
    }
    else if ( nVarsOld != nVars )
    {
        free( pPerms );
        nPerms = Extra_Factorial( nVars );   
        pPerms = Extra_Permutations( nVars );
        nVarsOld = nVars;
    }

    nMints    = (1 << nVars);
    uTruthMin = 0xFFFFFFFF;
    for ( i = 0; i < nMints; i++ )
    {
        uPhase = Extra_TruthPolarize( uTruth, i, nVars ); 
        for ( k = 0; k < nPerms; k++ )
        {
            uPerm = Extra_TruthPermute( uPhase, pPerms[k], nVars, 0 );
            if ( uTruthMin > uPerm )
                uTruthMin = uPerm;
        }
    }
    return uTruthMin;
}

/**Function*************************************************************

  Synopsis    [Computes NPN-canonical form using brute-force methods.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned Extra_TruthCanonNPN( unsigned uTruth, int nVars )
{
    static int nVarsOld, nPerms;
    static char ** pPerms = NULL;

    unsigned uTruthMin, uTruthC, uPhase, uPerm;
    int nMints, k, i;

    if ( pPerms == NULL )
    {
        nPerms = Extra_Factorial( nVars );   
        pPerms = Extra_Permutations( nVars );
        nVarsOld = nVars;
    }
    else if ( nVarsOld != nVars )
    {
        free( pPerms );
        nPerms = Extra_Factorial( nVars );   
        pPerms = Extra_Permutations( nVars );
        nVarsOld = nVars;
    }

    nMints    = (1 << nVars);
    uTruthC   = (unsigned)( (~uTruth) & ((~((unsigned)0)) >> (32-nMints)) );
    uTruthMin = 0xFFFFFFFF;
    for ( i = 0; i < nMints; i++ )
    {
        uPhase = Extra_TruthPolarize( uTruth, i, nVars ); 
        for ( k = 0; k < nPerms; k++ )
        {
            uPerm = Extra_TruthPermute( uPhase, pPerms[k], nVars, 0 );
            if ( uTruthMin > uPerm )
                uTruthMin = uPerm;
        }
        uPhase = Extra_TruthPolarize( uTruthC, i, nVars ); 
        for ( k = 0; k < nPerms; k++ )
        {
            uPerm = Extra_TruthPermute( uPhase, pPerms[k], nVars, 0 );
            if ( uTruthMin > uPerm )
                uTruthMin = uPerm;
        }
    }
    return uTruthMin;
}

/**Function*************************************************************

  Synopsis    [Computes NPN canonical forms for 4-variable functions.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Extra_Truth4VarNPN( unsigned short ** puCanons, char ** puPhases, char ** puPerms, unsigned char ** puMap )
{
    unsigned short * uCanons;
    unsigned char * uMap;
    unsigned uTruth, uPhase, uPerm;
    char ** pPerms4, * uPhases, * uPerms;
    int nFuncs, nClasses;
    int i, k;

    nFuncs  = (1 << 16);
    uCanons = ALLOC( unsigned short, nFuncs );
    uPhases = ALLOC( char, nFuncs );
    uPerms  = ALLOC( char, nFuncs );
    uMap    = ALLOC( unsigned char, nFuncs );
    memset( uCanons, 0, sizeof(unsigned short) * nFuncs );
    memset( uPhases, 0, sizeof(char) * nFuncs );
    memset( uPerms,  0, sizeof(char) * nFuncs );
    memset( uMap,    0, sizeof(unsigned char) * nFuncs );
    pPerms4 = Extra_Permutations( 4 );

    nClasses = 1;
    nFuncs = (1 << 15);
    for ( uTruth = 1; uTruth < (unsigned)nFuncs; uTruth++ )
    {
        // skip already assigned
        if ( uCanons[uTruth] )
        {
            assert( uTruth > uCanons[uTruth] );
            uMap[uTruth] = uMap[uCanons[uTruth]];
            continue;
        }
        uMap[uTruth] = nClasses++;
        for ( i = 0; i < 16; i++ )
        {
            uPhase = Extra_TruthPolarize( uTruth, i, 4 );
            for ( k = 0; k < 24; k++ )
            {
                uPerm = Extra_TruthPermute( uPhase, pPerms4[k], 4, 0 );
                if ( uCanons[uPerm] == 0 )
                {
                    uCanons[uPerm] = uTruth;
                    uPhases[uPerm] = i;
                    uPerms[uPerm]  = k;

                    uPerm = ~uPerm & 0xFFFF;
                    uCanons[uPerm] = uTruth;
                    uPhases[uPerm] = i | 16;
                    uPerms[uPerm]  = k;
                }
                else
                    assert( uCanons[uPerm] == uTruth );
            }
            uPhase = Extra_TruthPolarize( ~uTruth & 0xFFFF, i, 4 ); 
            for ( k = 0; k < 24; k++ )
            {
                uPerm = Extra_TruthPermute( uPhase, pPerms4[k], 4, 0 );
                if ( uCanons[uPerm] == 0 )
                {
                    uCanons[uPerm] = uTruth;
                    uPhases[uPerm] = i;
                    uPerms[uPerm]  = k;

                    uPerm = ~uPerm & 0xFFFF;
                    uCanons[uPerm] = uTruth;
                    uPhases[uPerm] = i | 16;
                    uPerms[uPerm]  = k;
                }
                else
                    assert( uCanons[uPerm] == uTruth );
            }
        }
    }
    uPhases[(1<<16)-1] = 16;
    assert( nClasses == 222 );
    free( pPerms4 );
    if ( puCanons ) 
        *puCanons = uCanons;
    else
        free( uCanons );
    if ( puPhases ) 
        *puPhases = uPhases;
    else
        free( uPhases );
    if ( puPerms ) 
        *puPerms = uPerms;
    else
        free( uPerms );
    if ( puMap ) 
        *puMap = uMap;
    else
        free( uMap );
}

/**Function*************************************************************

  Synopsis    [Computes NPN canonical forms for 4-variable functions.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Extra_Truth3VarN( unsigned ** puCanons, char *** puPhases, char ** ppCounters )
{
    int nPhasesMax = 8;
    unsigned * uCanons;
    unsigned uTruth, uPhase, uTruth32;
    char ** uPhases, * pCounters;
    int nFuncs, nClasses, i;

    nFuncs  = (1 << 8);
    uCanons = ALLOC( unsigned, nFuncs );
    memset( uCanons, 0, sizeof(unsigned) * nFuncs );
    pCounters = ALLOC( char, nFuncs );
    memset( pCounters, 0, sizeof(char) * nFuncs );
    uPhases = (char **)Extra_ArrayAlloc( nFuncs, nPhasesMax, sizeof(char) );
    nClasses = 0;
    for ( uTruth = 0; uTruth < (unsigned)nFuncs; uTruth++ )
    {
        // skip already assigned
        uTruth32 = ((uTruth << 24) | (uTruth << 16) | (uTruth << 8) | uTruth);
        if ( uCanons[uTruth] )
        {
            assert( uTruth32 > uCanons[uTruth] );
            continue;
        }
        nClasses++;
        for ( i = 0; i < 8; i++ )
        {
            uPhase = Extra_TruthPolarize( uTruth, i, 3 );
            if ( uCanons[uPhase] == 0 && (uTruth || i==0) )
            {
                uCanons[uPhase]    = uTruth32;
                uPhases[uPhase][0] = i;
                pCounters[uPhase]  = 1;
            }
            else
            {
                assert( uCanons[uPhase] == uTruth32 );
                if ( pCounters[uPhase] < nPhasesMax )
                    uPhases[uPhase][ pCounters[uPhase]++ ] = i;
            }
        }
    }
    if ( puCanons ) 
        *puCanons = uCanons;
    else
        free( uCanons );
    if ( puPhases ) 
        *puPhases = uPhases;
    else
        free( uPhases );
    if ( ppCounters ) 
        *ppCounters = pCounters;
    else
        free( pCounters );
    printf( "The number of 3N-classes = %d.\n", nClasses );
}

/**Function*************************************************************

  Synopsis    [Computes NPN canonical forms for 4-variable functions.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Extra_Truth4VarN( unsigned short ** puCanons, char *** puPhases, char ** ppCounters, int nPhasesMax )
{
    unsigned short * uCanons;
    unsigned uTruth, uPhase;
    char ** uPhases, * pCounters;
    int nFuncs, nClasses, i;

    nFuncs  = (1 << 16);
    uCanons = ALLOC( unsigned short, nFuncs );
    memset( uCanons, 0, sizeof(unsigned short) * nFuncs );
    pCounters = ALLOC( char, nFuncs );
    memset( pCounters, 0, sizeof(char) * nFuncs );
    uPhases = (char **)Extra_ArrayAlloc( nFuncs, nPhasesMax, sizeof(char) );
    nClasses = 0;
    for ( uTruth = 0; uTruth < (unsigned)nFuncs; uTruth++ )
    {
        // skip already assigned
        if ( uCanons[uTruth] )
        {
            assert( uTruth > uCanons[uTruth] );
            continue;
        }
        nClasses++;
        for ( i = 0; i < 16; i++ )
        {
            uPhase = Extra_TruthPolarize( uTruth, i, 4 );
            if ( uCanons[uPhase] == 0 && (uTruth || i==0) )
            {
                uCanons[uPhase]    = uTruth;
                uPhases[uPhase][0] = i;
                pCounters[uPhase]  = 1;
            }
            else
            {
                assert( uCanons[uPhase] == uTruth );
                if ( pCounters[uPhase] < nPhasesMax )
                    uPhases[uPhase][ pCounters[uPhase]++ ] = i;
            }
        }
    }
    if ( puCanons ) 
        *puCanons = uCanons;
    else
        free( uCanons );
    if ( puPhases ) 
        *puPhases = uPhases;
    else
        free( uPhases );
    if ( ppCounters ) 
        *ppCounters = pCounters;
    else
        free( pCounters );
    printf( "The number of 4N-classes = %d.\n", nClasses );
}

/**Function*************************************************************

  Synopsis    [Allocated one-memory-chunk array.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void ** Extra_ArrayAlloc( int nCols, int nRows, int Size )
{
    char ** pRes;
    char * pBuffer;
    int i;
    assert( nCols > 0 && nRows > 0 && Size > 0 );
    pBuffer = ALLOC( char, nCols * (sizeof(void *) + nRows * Size) );
    pRes = (char **)pBuffer;
    pRes[0] = pBuffer + nCols * sizeof(void *);
    for ( i = 1; i < nCols; i++ )
        pRes[i] = pRes[0] + i * nRows * Size;
    return pRes;
}

/**Function*************************************************************

  Synopsis    [Computes a phase of the 3-var function.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned short Extra_TruthPerm4One( unsigned uTruth, int Phase )
{
    // cases
    static unsigned short Cases[16] = {
        0,      // 0000  - skip
        0,      // 0001  - skip
        0xCCCC, // 0010  - single var
        0,      // 0011  - skip
        0xF0F0, // 0100  - single var
        1,      // 0101
        1,      // 0110
        0,      // 0111  - skip
        0xFF00, // 1000  - single var
        1,      // 1001
        1,      // 1010
        1,      // 1011
        1,      // 1100
        1,      // 1101
        1,      // 1110
        0       // 1111  - skip
    };
    // permutations
    static int Perms[16][4] = {
        { 0, 0, 0, 0 }, // 0000  - skip
        { 0, 0, 0, 0 }, // 0001  - skip
        { 0, 0, 0, 0 }, // 0010  - single var
        { 0, 0, 0, 0 }, // 0011  - skip
        { 0, 0, 0, 0 }, // 0100  - single var
        { 0, 2, 1, 3 }, // 0101
        { 2, 0, 1, 3 }, // 0110
        { 0, 0, 0, 0 }, // 0111  - skip
        { 0, 0, 0, 0 }, // 1000  - single var
        { 0, 2, 3, 1 }, // 1001
        { 2, 0, 3, 1 }, // 1010
        { 0, 1, 3, 2 }, // 1011
        { 2, 3, 0, 1 }, // 1100
        { 0, 3, 1, 2 }, // 1101
        { 3, 0, 1, 2 }, // 1110
        { 0, 0, 0, 0 }  // 1111  - skip
    };
    int i, k, iRes;
    unsigned uTruthRes;
    assert( Phase >= 0 && Phase < 16 );
    if ( Cases[Phase] == 0 )
        return uTruth;
    if ( Cases[Phase] > 1 )
        return Cases[Phase];
    uTruthRes = 0;
    for ( i = 0; i < 16; i++ )
        if ( uTruth & (1 << i) )
        {
            for ( iRes = 0, k = 0; k < 4; k++ )
                if ( i & (1 << Perms[Phase][k]) )
                    iRes |= (1 << k);
            uTruthRes |= (1 << iRes);
        }
    return uTruthRes;
}

/**Function*************************************************************

  Synopsis    [Computes a phase of the 3-var function.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned Extra_TruthPerm5One( unsigned uTruth, int Phase )
{
    // cases
    static unsigned Cases[32] = {
        0,          // 00000  - skip
        0,          // 00001  - skip
        0xCCCCCCCC, // 00010  - single var
        0,          // 00011  - skip
        0xF0F0F0F0, // 00100  - single var
        1,          // 00101
        1,          // 00110
        0,          // 00111  - skip
        0xFF00FF00, // 01000  - single var
        1,          // 01001
        1,          // 01010
        1,          // 01011
        1,          // 01100
        1,          // 01101
        1,          // 01110
        0,          // 01111  - skip
        0xFFFF0000, // 10000  - skip
        1,          // 10001
        1,          // 10010
        1,          // 10011
        1,          // 10100
        1,          // 10101
        1,          // 10110
        1,          // 10111  - four var
        1,          // 11000
        1,          // 11001
        1,          // 11010
        1,          // 11011  - four var
        1,          // 11100
        1,          // 11101  - four var
        1,          // 11110  - four var
        0           // 11111  - skip
    };
    // permutations
    static int Perms[32][5] = {
        { 0, 0, 0, 0, 0 }, // 00000  - skip
        { 0, 0, 0, 0, 0 }, // 00001  - skip
        { 0, 0, 0, 0, 0 }, // 00010  - single var
        { 0, 0, 0, 0, 0 }, // 00011  - skip
        { 0, 0, 0, 0, 0 }, // 00100  - single var
        { 0, 2, 1, 3, 4 }, // 00101
        { 2, 0, 1, 3, 4 }, // 00110
        { 0, 0, 0, 0, 0 }, // 00111  - skip
        { 0, 0, 0, 0, 0 }, // 01000  - single var
        { 0, 2, 3, 1, 4 }, // 01001
        { 2, 0, 3, 1, 4 }, // 01010
        { 0, 1, 3, 2, 4 }, // 01011
        { 2, 3, 0, 1, 4 }, // 01100
        { 0, 3, 1, 2, 4 }, // 01101
        { 3, 0, 1, 2, 4 }, // 01110
        { 0, 0, 0, 0, 0 }, // 01111  - skip
        { 0, 0, 0, 0, 0 }, // 10000  - single var
        { 0, 4, 2, 3, 1 }, // 10001
        { 4, 0, 2, 3, 1 }, // 10010
        { 0, 1, 3, 4, 2 }, // 10011
        { 2, 3, 0, 4, 1 }, // 10100
        { 0, 3, 1, 4, 2 }, // 10101
        { 3, 0, 1, 4, 2 }, // 10110
        { 0, 1, 2, 4, 3 }, // 10111  - four var
        { 2, 3, 4, 0, 1 }, // 11000
        { 0, 3, 4, 1, 2 }, // 11001
        { 3, 0, 4, 1, 2 }, // 11010
        { 0, 1, 4, 2, 3 }, // 11011  - four var
        { 3, 4, 0, 1, 2 }, // 11100
        { 0, 4, 1, 2, 3 }, // 11101  - four var
        { 4, 0, 1, 2, 3 }, // 11110  - four var
        { 0, 0, 0, 0, 0 }  // 11111  - skip
    };
    int i, k, iRes;
    unsigned uTruthRes;
    assert( Phase >= 0 && Phase < 32 );
    if ( Cases[Phase] == 0 )
        return uTruth;
    if ( Cases[Phase] > 1 )
        return Cases[Phase];
    uTruthRes = 0;
    for ( i = 0; i < 32; i++ )
        if ( uTruth & (1 << i) )
        {
            for ( iRes = 0, k = 0; k < 5; k++ )
                if ( i & (1 << Perms[Phase][k]) )
                    iRes |= (1 << k);
            uTruthRes |= (1 << iRes);
        }
    return uTruthRes;
}

/**Function*************************************************************

  Synopsis    [Computes a phase of the 3-var function.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Extra_TruthPerm6One( unsigned * uTruth, int Phase, unsigned * uTruthRes )
{
    // cases
    static unsigned Cases[64] = {
        0,          // 000000  - skip
        0,          // 000001  - skip
        0xCCCCCCCC, // 000010  - single var
        0,          // 000011  - skip
        0xF0F0F0F0, // 000100  - single var
        1,          // 000101
        1,          // 000110
        0,          // 000111  - skip
        0xFF00FF00, // 001000  - single var
        1,          // 001001
        1,          // 001010
        1,          // 001011
        1,          // 001100
        1,          // 001101
        1,          // 001110
        0,          // 001111  - skip
        0xFFFF0000, // 010000  - skip
        1,          // 010001
        1,          // 010010
        1,          // 010011
        1,          // 010100
        1,          // 010101
        1,          // 010110
        1,          // 010111  - four var
        1,          // 011000
        1,          // 011001
        1,          // 011010
        1,          // 011011  - four var
        1,          // 011100
        1,          // 011101  - four var
        1,          // 011110  - four var
        0,          // 011111  - skip
        0xFFFFFFFF, // 100000  - single var
        1,          // 100001 
        1,          // 100010  
        1,          // 100011  
        1,          // 100100  
        1,          // 100101
        1,          // 100110
        1,          // 100111  
        1,          // 101000  
        1,          // 101001
        1,          // 101010
        1,          // 101011
        1,          // 101100
        1,          // 101101
        1,          // 101110
        1,          // 101111  
        1,          // 110000  
        1,          // 110001
        1,          // 110010
        1,          // 110011
        1,          // 110100
        1,          // 110101
        1,          // 110110
        1,          // 110111  
        1,          // 111000
        1,          // 111001
        1,          // 111010
        1,          // 111011  
        1,          // 111100
        1,          // 111101  
        1,          // 111110  
        0           // 111111  - skip
    };
    // permutations
    static int Perms[64][6] = {
        { 0, 0, 0, 0, 0, 0 }, // 000000  - skip
        { 0, 0, 0, 0, 0, 0 }, // 000001  - skip
        { 0, 0, 0, 0, 0, 0 }, // 000010  - single var
        { 0, 0, 0, 0, 0, 0 }, // 000011  - skip
        { 0, 0, 0, 0, 0, 0 }, // 000100  - single var
        { 0, 2, 1, 3, 4, 5 }, // 000101
        { 2, 0, 1, 3, 4, 5 }, // 000110
        { 0, 0, 0, 0, 0, 0 }, // 000111  - skip
        { 0, 0, 0, 0, 0, 0 }, // 001000  - single var
        { 0, 2, 3, 1, 4, 5 }, // 001001
        { 2, 0, 3, 1, 4, 5 }, // 001010
        { 0, 1, 3, 2, 4, 5 }, // 001011
        { 2, 3, 0, 1, 4, 5 }, // 001100
        { 0, 3, 1, 2, 4, 5 }, // 001101
        { 3, 0, 1, 2, 4, 5 }, // 001110
        { 0, 0, 0, 0, 0, 0 }, // 001111  - skip
        { 0, 0, 0, 0, 0, 0 }, // 010000  - skip
        { 0, 4, 2, 3, 1, 5 }, // 010001
        { 4, 0, 2, 3, 1, 5 }, // 010010
        { 0, 1, 3, 4, 2, 5 }, // 010011
        { 2, 3, 0, 4, 1, 5 }, // 010100
        { 0, 3, 1, 4, 2, 5 }, // 010101
        { 3, 0, 1, 4, 2, 5 }, // 010110
        { 0, 1, 2, 4, 3, 5 }, // 010111  - four var
        { 2, 3, 4, 0, 1, 5 }, // 011000
        { 0, 3, 4, 1, 2, 5 }, // 011001
        { 3, 0, 4, 1, 2, 5 }, // 011010
        { 0, 1, 4, 2, 3, 5 }, // 011011  - four var
        { 3, 4, 0, 1, 2, 5 }, // 011100
        { 0, 4, 1, 2, 3, 5 }, // 011101  - four var
        { 4, 0, 1, 2, 3, 5 }, // 011110  - four var
        { 0, 0, 0, 0, 0, 0 }, // 011111  - skip
        { 0, 0, 0, 0, 0, 0 }, // 100000  - single var
        { 0, 2, 3, 4, 5, 1 }, // 100001 
        { 2, 0, 3, 4, 5, 1 }, // 100010  
        { 0, 1, 3, 4, 5, 2 }, // 100011  
        { 2, 3, 0, 4, 5, 1 }, // 100100  
        { 0, 3, 1, 4, 5, 2 }, // 100101
        { 3, 0, 1, 4, 5, 2 }, // 100110
        { 0, 1, 2, 4, 5, 3 }, // 100111  
        { 2, 3, 4, 0, 5, 1 }, // 101000  
        { 0, 3, 4, 1, 5, 2 }, // 101001
        { 3, 0, 4, 1, 5, 2 }, // 101010
        { 0, 1, 4, 2, 5, 3 }, // 101011
        { 3, 4, 0, 1, 5, 2 }, // 101100
        { 0, 4, 1, 2, 5, 3 }, // 101101
        { 4, 0, 1, 2, 5, 3 }, // 101110
        { 0, 1, 2, 3, 5, 4 }, // 101111  
        { 2, 3, 4, 5, 0, 1 }, // 110000  
        { 0, 3, 4, 5, 1, 2 }, // 110001
        { 3, 0, 4, 5, 1, 2 }, // 110010
        { 0, 1, 4, 5, 2, 3 }, // 110011
        { 3, 4, 0, 5, 1, 2 }, // 110100
        { 0, 4, 1, 5, 2, 3 }, // 110101
        { 4, 0, 1, 5, 2, 3 }, // 110110
        { 0, 1, 2, 5, 3, 4 }, // 110111  
        { 3, 4, 5, 0, 1, 2 }, // 111000
        { 0, 4, 5, 1, 2, 3 }, // 111001
        { 4, 0, 5, 1, 2, 3 }, // 111010
        { 0, 1, 5, 2, 3, 4 }, // 111011  
        { 4, 5, 0, 1, 2, 3 }, // 111100
        { 0, 5, 1, 2, 3, 4 }, // 111101  
        { 5, 0, 1, 2, 3, 4 }, // 111110  
        { 0, 0, 0, 0, 0, 0 }  // 111111  - skip
    };
    int i, k, iRes;
    assert( Phase >= 0 && Phase < 64 );
    if ( Cases[Phase] == 0 )
    {
        uTruthRes[0] = uTruth[0];
        uTruthRes[1] = uTruth[1];
        return;
    }
    if ( Cases[Phase] > 1 )
    {
        if ( Phase == 32 )
        {
            uTruthRes[0] = 0x00000000;
            uTruthRes[1] = 0xFFFFFFFF;
        }
        else
        {
            uTruthRes[0] = Cases[Phase];
            uTruthRes[1] = Cases[Phase];
        }
        return;
    }
    uTruthRes[0] = 0;
    uTruthRes[1] = 0;
    for ( i = 0; i < 64; i++ )
    {
        if ( i < 32 )
        {
            if ( uTruth[0] & (1 << i) )
            {
                for ( iRes = 0, k = 0; k < 6; k++ )
                    if ( i & (1 << Perms[Phase][k]) )
                        iRes |= (1 << k);
                if ( iRes < 32 )
                    uTruthRes[0] |= (1 << iRes);
                else
                    uTruthRes[1] |= (1 << (iRes-32));
            }
        }
        else
        {
            if ( uTruth[1] & (1 << (i-32)) )
            {
                for ( iRes = 0, k = 0; k < 6; k++ )
                    if ( i & (1 << Perms[Phase][k]) )
                        iRes |= (1 << k);
                if ( iRes < 32 )
                    uTruthRes[0] |= (1 << iRes);
                else
                    uTruthRes[1] |= (1 << (iRes-32));
            }
        }
    }
}

/**Function*************************************************************

  Synopsis    [Allocated lookup table for truth table permutation.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned short ** Extra_TruthPerm43()
{
    unsigned short ** pTable;
    unsigned uTruth;
    int i, k;
    pTable = (unsigned short **)Extra_ArrayAlloc( 256, 16, 2 );
    for ( i = 0; i < 256; i++ )
    {
        uTruth = (i << 8) | i;
        for ( k = 0; k < 16; k++ )
            pTable[i][k] = Extra_TruthPerm4One( uTruth, k );
    }
    return pTable;
}

/**Function*************************************************************

  Synopsis    [Allocated lookup table for truth table permutation.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned ** Extra_TruthPerm53()
{
    unsigned ** pTable;
    unsigned uTruth;
    int i, k;
    pTable = (unsigned **)Extra_ArrayAlloc( 256, 32, 4 );
    for ( i = 0; i < 256; i++ )
    {
        uTruth = (i << 24) | (i << 16) | (i << 8) | i;
        for ( k = 0; k < 32; k++ )
            pTable[i][k] = Extra_TruthPerm5One( uTruth, k );
    }
    return pTable;
}

/**Function*************************************************************

  Synopsis    [Allocated lookup table for truth table permutation.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned ** Extra_TruthPerm54()
{
    unsigned ** pTable;
    unsigned uTruth;
    int i;
    pTable = (unsigned **)Extra_ArrayAlloc( 256*256, 4, 4 );
    for ( i = 0; i < 256*256; i++ )
    {
        uTruth = (i << 16) | i;
        pTable[i][0] = Extra_TruthPerm5One( uTruth, 31-8 );
        pTable[i][1] = Extra_TruthPerm5One( uTruth, 31-4 );
        pTable[i][2] = Extra_TruthPerm5One( uTruth, 31-2 );
        pTable[i][3] = Extra_TruthPerm5One( uTruth, 31-1 );
    }
    return pTable;
}

/**Function*************************************************************

  Synopsis    [Allocated lookup table for truth table permutation.]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned ** Extra_TruthPerm63()
{
    unsigned ** pTable;
    unsigned uTruth[2];
    int i, k;
    pTable = (unsigned **)Extra_ArrayAlloc( 256, 64, 8 );
    for ( i = 0; i < 256; i++ )
    {
        uTruth[0] = (i << 24) | (i << 16) | (i << 8) | i;
        uTruth[1] = uTruth[0];
        for ( k = 0; k < 64; k++ )
            Extra_TruthPerm6One( uTruth, k, &pTable[i][k] );
    }
    return pTable;
}

/**Function*************************************************************

  Synopsis    [Returns the smallest prime larger than the number.]

  Description []

  SideEffects []

  SeeAlso     []

***********************************************************************/
unsigned int Cudd_PrimeCopy( unsigned int  p)
{
    int i,pn;

    p--;
    do {
        p++;
        if (p&1) {
        pn = 1;
        i = 3;
        while ((unsigned) (i * i) <= p) {
        if (p % i == 0) {
            pn = 0;
            break;
        }
        i += 2;
        }
    } else {
        pn = 0;
    }
    } while (!pn);
    return(p);

} /* end of Cudd_Prime */


/*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Definition of static Functions                                            */
/*---------------------------------------------------------------------------*/


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


