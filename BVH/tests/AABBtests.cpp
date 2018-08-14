#include "gmock/gmock.h"
#include "../BV.h"


class AABBTestData : public ::testing::Test
{
    public:

    TRI *t1, *t2;
    BOND *s1, *s2;
    POINT *a, *b, *c, *d, *e, *f;

    FT_TRI *T1, *T2;
    FT_BOND *B1, *B2;
    FT_POINT* P;

    //can probably make this a static StartUp/TearDown method
    //in the derived class fixture so share the resources.
    AABBTestData()
    {
        a = new POINT;         b = new POINT;
        Coords(a)[0] = 2.0;    Coords(b)[0] = 1.0;
        Coords(a)[1] = 2.0;    Coords(b)[1] = 1.0;
        Coords(a)[2] = 3.0;    Coords(b)[2] = 1.0;

        c = new POINT;          d = new POINT;
        Coords(c)[0] = 9.0;     Coords(d)[0] = 10.0;
        Coords(c)[1] = 0.5;     Coords(d)[1] = 0.0;
        Coords(c)[2] = 2.0;     Coords(d)[2] = 0.0;

        e = new POINT;          f = new POINT;
        Coords(e)[0] = 0.0;     Coords(f)[0] = 0.0;
        Coords(e)[1] = 10.0;    Coords(f)[1] = 0.0;
        Coords(e)[2] = 0.0;     Coords(f)[2] = 10.0;

        t1 = new TRI;                t2 = new TRI;
        Point_of_tri(t1)[0] = a;     Point_of_tri(t2)[0] = d;
        Point_of_tri(t1)[1] = b;     Point_of_tri(t2)[1] = e;
        Point_of_tri(t1)[2] = c;     Point_of_tri(t2)[2] = f;

        s1 = new BOND;      s2 = new BOND;
        s1->start = a;      s2->start = c;
        s1->end = b;        s2->end = f;

        T1 = new FT_TRI(t1);    T2 = new FT_TRI(t2);
        B1 = new FT_BOND(s1);   B2 = new FT_BOND(s2);
        P = new FT_POINT(a);
    }

    virtual void TearDown()
    {
        delete a; delete b; delete c;
        delete d; delete e; delete f;
        delete P; delete s1; delete s2;
        delete t1; delete t2; delete B1;
        delete B2; delete T1; delete T2;
    }

    virtual ~AABBTestData() = default;

};

class AABBTests : public AABBTestData
{
    public:
    
    AABB *bbT1, *bbT2, *bbB1, *bbB2;

    AABBTests()
        : AABBTestData{}
    {
        bbT1 = new AABB(T1);
        bbT2 = new AABB(T2);
        bbB1 = new AABB(B1);
        bbB2 = new AABB(B2);
    }

    void TearDown() override
    {
        delete bbT1; delete bbT2;
        delete bbB1; delete bbB2;
        AABBTestData::TearDown();
    }

    ~AABBTests() = default;

};


TEST_F(AABBTests, BoxesOverlapVsContain)
{
    //contains means strictly contained
    ASSERT_TRUE(bbT2->contains(bbT1));
    ASSERT_FALSE(bbT1->contains(bbB1));

    //shared surface is considered overlap
    ASSERT_TRUE(bbT1->overlaps(bbB1));
    ASSERT_TRUE(bbT1->overlaps(bbB2));
}

TEST_F(AABBTests, Constructor_FT_HSE)
{
    ASSERT_EQ(bbT1->getTypeBV(),BV_Type::AABB);
}

//TODO: Degenerate Cases?:
//      
//      1.  Box has zero volume for a point;
//          should AABB be allowed to be given
//          a point as a constructor argument?
//      
//      2.  What about for bonds or tris that are parallel
//          to two axis simultaneously and box is 2d?
//          (volume = 0)
//      
//      3.  Is volume even necessary, since using
//          hilbert curve for BVH construction?
//          




