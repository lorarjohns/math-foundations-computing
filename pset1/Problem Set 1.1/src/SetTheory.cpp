#include "SetTheory.h"
#include <stdexcept>
#include <iostream>

using namespace std;


/**
* Evaluates true or false: S in T?
*
* If T is a set, say whether S is an element
* of T.
* @param S: Object to be tested for membership
* in T
* @param T: Object that may or may not be a set
* @return bool representing whether S in T, or
* false if T is not a set
*/
bool isElementOf(Object S, Object T) {

    bool isElementOfT = false;

    if (isSet(T)) {
        if (asSet(T).count(S) == 1) {
            isElementOfT = true;
        }
    }
    return isElementOfT;
}

/**
* Evaluates true or false: S is a subset of T?
*
* If S and T are sets, say whether S is a subset
* of T.
* @param S: Object to be tested for set type
* and subset relation.
* @param T: Object to be tested for set type
* and superset relation.
* @return bool representing whether S in T, or
* false if either S or T is not a set
*/
bool isSubsetOf(Object S, Object T) {
    if (!isSet(S) || !isSet(T)) {
        return false;
    }
    bool isSubset = true;
    for (auto elem: asSet(S)) {
        if (!isElementOf(elem, T)) {
            isSubset = false;
            break;
        }
    }
    return isSubset;
}

/** True or false: S and T are sets, and S n T = emptyset?
*
* TODO: Optimize this algorithm
* If none of the elements in S are in T
* and vice versa, return true. Else, return
* false (including when S or T is not a set)
*
* @param S first set to compare
* @param T second set to compare
* @return bool representing whether S and T
* are disjoint sets
*/
bool areDisjointSets(Object S, Object T) {
    bool disjointTS;
    bool disjointST;

    if (!isSet(S) || !isSet(T)) {
        return false;
    }

    disjointST = true;
    for (auto elem: asSet(S)) {
        if (isElementOf(elem, T)) {
            disjointST = false;
            break;
        }
    }

    disjointTS = true;
    for (auto elem: asSet(T)) {
        if (isElementOf(elem, S)) {
            disjointTS = false;
            break;
        }
    }

    return disjointST && disjointTS;
}

/** True or false: S = {T}?
*
* @param S: Object to check for set type
* and singleton set membership
* @param T: Object to check for set type
* and singleton set candidate
* @return bool representing whether S is the
* only element of T, or false if S or T is
* not a set
*/
bool isSingletonOf(Object S, Object T) {
    bool singleton = false;
    // if T is a set and S is a set
    if (!isSet(T)) {
        return false;
    }
    if (!isSet(S)) {
        return false;
    }
    if (asSet(S).size() == 1 && isElementOf(T, S)) {
    //if (isElementOf(T, S)) {

    singleton = true;
        // if T has only one element
        // and it is equal to S
        // return true
        // else return false

    }
    return singleton;
    }


/** True or false: S and T are sets, and S in P(T)?
*
* If S in P(T), then S is a subset of T.
*
* @param S: Object to be tested for set type
* and power set membership.
* @param T: Object to be tested for set type
* and superset relation.
* @return bool representing whether S in P(T), or
* false if either S or T is not a set
*/
bool isElementOfPowerSet(Object S, Object T) {
    return isSubsetOf(S, T);
}


/** True or false: S and T are sets, and S is a subset of P(T)?
 *
 * If every _element_ e in S is a _subset_ of T, then e is an
 * _element_ of P(T), and consequently S is a subset of P(T).
 * @param S Object to test for set type and candidate
 * for powerset subset
 * @param T Object to test for set type and "generate" power set for
 * (we're not really generating the full power set in memory)
 * @return bool representing whether S is a subset of P(T), or false
 * if S or T is not a set
*/
bool isSubsetOfPowerSet(Object S, Object T) {
    bool allElemsOfSAreSubsetsOfT = false;
    if (!isSet(S) || !isSet(T)) {
        return false;
    }
    // if all elements in S are subsets of T, then it is a subset of P(T)
    allElemsOfSAreSubsetsOfT = true;
    for (auto elem: asSet(S)) {
        if (!isSubsetOf(elem, T)) {
            allElemsOfSAreSubsetsOfT = false;
            break;
        }
    }
    return allElemsOfSAreSubsetsOfT;
}

/** True or false: S and T are sets, and S is a subset of P(P(T))?
 *
 * If every element p of S is a _subset_ of P(T), then p is an _element_
 * of P(P(T)), and consequently S is a subset of P(P(T)).
 *
 * A subset of P(T) is a set made up of elements e that each represent
 * a _subset_ of T.
 *
 * @param S Object to test for set type and candidate
 * for powerset subset
 * @param T Object to test for set type and "generate" power set for
 * (we're not really generating the full power set in memory)
 * @return bool representing whether S is a subset of P(T), or false
 * if S or T is not a set
*/

bool isSubsetOfDoublePowerSet(Object S, Object T) {
    bool isSubsetDPS = false;
    // for each element in S (a set of subsets of P(T))
    //  for each sub-element in element (a set of subsets of T)
    //      for each sub-sub-element in sub-element (subsets of T)
    //          if sub-sub-element isSubSetOf T
    //              keep going (else break)

    if (!isSet(S) || !isSet(T)) {
        return false;
    }

    isSubsetDPS = true;
    for (auto setSubPT: asSet(S)) {
        for (auto setSubT: asSet(setSubPT)) {
            for (auto subT: asSet(setSubT)) {
                if (!isSubsetOf(subT, T)) {
                    isSubsetDPS = false;
                    break;
                }
            }
        }
    }
    return isSubsetDPS;
}
