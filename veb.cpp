#include "veb.hpp"

TvEB::TvEB ( int UniverseSize )
        : Universe( roundingPower2 ( UniverseSize ) ), uniSqrt ( sqrt ( Universe) ),
          lowerUniSqrt ( sqrtLow ( Universe) ), higherUniSqrt ( sqrtHigh ( Universe) ),
          min ( UNDEFINED ), max ( UNDEFINED ), summary ( NULL )
{
    if ( UniverseSize <= 0 )
    {
        std::cerr << "universe size of TvEB must be bigger than 0" << std::endl;
        return;
    }

    if ( Universe> 2 )
    {
        cluster = new TvEB * [higherUniSqrt];
        for ( int i = 0; i < higherUniSqrt; ++i )
        {
            cluster[i] = NULL;
        }
    }
    else
    {
        cluster = NULL;
    }
}

void vEB_print ( TvEB * tree, std::ostream & os )
{
    if ( !tree ) return;
    os << "tree: " << tree << std::endl;
    os << "min: " << tree->min << ", max: " << tree->max << std::endl;
    os << "uni: " << tree->Universe<< ", uniSqrt: " << tree->uniSqrt << std::endl;
    os << "lowerUniSqrt: " << tree->lowerUniSqrt;
    os << ", higherUniSqrt: " << tree->higherUniSqrt << std::endl;
    os << "summary: " << tree->summary << std::endl;
    if ( tree->Universe> 2 )
    {
        for ( int i = 0; i < tree->higherUniSqrt; ++i )
        {
            os << "cluster " << i << ": " << tree->cluster[i] << std::endl;
        }
    }
    else
    {
        os << "cluster " << tree->cluster << std::endl;
    }
}


TvEB::~TvEB()
{
    if ( summary ) deletevEB summary;
    if ( cluster )
    {
        for ( int i = 0; i < higherUniSqrt; ++i )
        {
            if ( cluster[i] ) deletevEB cluster[i];
        }
        deletevEB [] cluster;
    }
}

int roundingPower2 ( int x )
{
    if ( x < 0 ) return 0;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}


bool min ( TvEB * tree, int & res )
{
    if ( tree )
    {
        res = tree->min;
        return true;
    }
    return false;
}

bool max ( TvEB * tree, int & res )
{
    if ( tree )
    {
        res = tree->max;
        return true;
    }
    return false;
}

float sqrtLow ( int val )
{
    return pow ( 2, floor ( log2 ( val )  / 2 ) );
}

float sqrtHigh ( int val )
{
    return pow ( 2, ceil ( log2 ( val )  / 2 ) );
}

int low ( TvEB * tree, int val )
{
    return val % ( int ) sqrtLow ( tree->Universe);
}

int high ( TvEB * tree, int val )
{
    return val / sqrtLow ( tree->Universe);
}

int index ( TvEB * tree, int high, int low )
{
    return high * sqrtLow ( tree->Universe) + low;
}

bool insert ( TvEB *& tree, int val, int parentUniSqrt )
{
    if ( !tree )
    {
        tree = new TvEB ( parentUniSqrt );
    }

#ifdef DEBUG
    DEBUG_OS << "inserting " << val << " to tree " << tree
           << " of size " << tree->Universe<< DEBUG_OS_ENDL;
#endif /* DEBUG */

    if ( val < 0 || val >= tree->Universe) return false;

    if ( tree->min == val || tree->max == val ) return false;

    if ( tree->min == UNDEFINED )
    {
        tree->min = tree->max = val;
        return true;
    }

    if ( val < tree->min )
    {
        int tmp = val;
        val = tree->min;
        tree->min = tmp;
    }

    if ( val > tree->max )
    {
        tree->max = val;
    }

    if ( tree->Universe> 2 )
    {
        int lowVal = low ( tree, val );
        int highVal = high ( tree, val );
        if ( !tree->cluster[highVal] )
        {
            if ( !insert ( tree->summary, highVal, tree->higherUniSqrt ) ) return false;
        }

        if ( !insert ( tree->cluster[highVal], lowVal, tree->lowerUniSqrt ) ) return false;
    }
    return true;
}

bool deletevEB ( TvEB *& tree, int val )
{
    if ( !tree ) return false;

#ifdef DEBUG
    DEBUG_OS << "deleting " << val << " from tree " << tree
           << " of size " << tree->Universe<< DEBUG_OS_ENDL;
#endif /* DEBUG */

    if ( val < 0 || val >= tree->Universe) return false;
    if ( tree->min > val || tree->max < val ) return false;

    if ( tree->min == val )
    {
        int i;
        if ( !min ( tree->summary, i ) || i == UNDEFINED )
        {
            if ( tree->min != tree->max )
            {
                tree->min = tree->max;
                return true;
            }

            tree->min = tree->max = UNDEFINED;
            deletevEB tree;
            tree = NULL;
            return true;
        }

        val = tree->min = index ( tree, i, tree->cluster[i]->min );
    }

    if ( tree->Universe> 2 )
    {
        int highVal = high ( tree, val );
        if ( !deletevEB ( tree->cluster[highVal], low ( tree, val ) ) ) return false;

        int tmp;
        if ( !min ( tree->cluster[highVal], tmp ) || tmp == UNDEFINED )
        {
            if ( !deletevEB ( tree->summary, highVal ) ) return false;
        }
    }

    if ( tree->max == val )
    {
        int tmp;
        if ( !max ( tree->summary, tmp ) || tmp == UNDEFINED )
        {
            tree->max = tree->min;
        }
        else
        {
            int i;
            if ( !max ( tree->summary, i ) ) return false;
            tree->max = index ( tree, i, tree->cluster[i]->max );
        }
    }
    return true;
}

bool vEB_find ( TvEB * tree, int val )
{
    if ( !tree ) return false;

#ifdef DEBUG
    DEBUG_OS << "looking for " << val << " in tree " << tree << DEBUG_OS_ENDL;
#endif /* DEBUG */

    if ( val < 0 || val >= tree->Universe) return false;
    if ( tree->min > val || tree->max < val ) return false;
    if ( tree->min == val ) return true;
    if ( !tree->summary )
    {
        return tree->max == val;
    }
    if ( !vEB_find ( tree->cluster[high ( tree, val )], low ( tree, val ) ) )
        return false;
    return true;
}

bool vEB_succ ( TvEB * tree, int val, int & res )
{
    if ( !tree ) return false;

#ifdef DEBUG
    DEBUG_OS << "looking for successor of " << val << " in tree " << tree
           << " of size " << tree->Universe<< DEBUG_OS_ENDL;
#endif /* DEBUG */

    if ( val < -1 || val >= tree->Universe) return false;

    if ( tree->min > val )
    {
        res = tree->min;
        return true;
    }

    if ( !tree->summary )
    {
        if ( tree->max > val )
        {
            res = tree->max;
            return true;
        }
        return false;
    }

    int lowVal = low ( tree, val );
    int highVal = high ( tree, val );
    int i = highVal;
    int j = UNDEFINED;
    int tmp;
    if ( max ( tree->cluster[i], tmp ) && lowVal < tmp )
    {
        if ( !vEB_succ ( tree->cluster[i], lowVal, j ) ) return false;
    }
    else
    {
        if ( !vEB_succ ( tree->summary, highVal, i ) )
        {
            if ( tree->max > val )
            {
                res = tree->max;
                return true;
            }
            return false;
        }
        if ( !min ( tree->cluster[i], j ) ) return false;
    }

    res = index ( tree, i, j );
    return true;
}

bool vEB_pred ( TvEB * tree, int val, int & res )
{
    if ( !tree ) return false;

#ifdef DEBUG
    DEBUG_OS << "looking for predecessor of " << val << " in tree " << tree
           << " of size " << tree->Universe<< DEBUG_OS_ENDL;
#endif /* DEBUG */

    if ( val < 0 || val > tree->Universe) return false;

    if ( tree->max < val )
    {
        res = tree->max;
        return true;
    }

    if ( !tree->summary )
    {
        if ( tree->min < val )
        {
            res = tree->min;
            return true;
        }
        return false;
    }

    int lowVal = low ( tree, val );
    int highVal = high ( tree, val );
    int i = highVal;
    int j = UNDEFINED;
    int tmp;
    if ( min ( tree->cluster[i], tmp ) && lowVal > tmp )
    {
        if ( !vEB_pred ( tree->cluster[i], lowVal, j ) ) return false;
    }
    else
    {
        if ( !vEB_pred ( tree->summary, highVal, i ) )
        {
            if ( tree->min < val )
            {
                res = tree->min;
                return true;
            }
            return false;
        }
        if ( !max ( tree->cluster[i], j ) ) return false;
    }

    res = index ( tree, i, j );
    return true;
}

