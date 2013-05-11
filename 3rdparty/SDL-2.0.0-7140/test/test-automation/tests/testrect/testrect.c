/**
 * Original code: automated SDL rect test written by Edgar Simo "bobbens"
 * New/updated tests: aschiffler at ferzkopp dot net
 */

#include <stdio.h>

#include <SDL/SDL.h>

#include "../../include/SDL_test.h"

/* SDL_IntersectRectAndLine */
static const TestCaseReference test1 =
		(TestCaseReference){ "rect_testIntersectRectAndLine", "Tests SDL_IntersectRectAndLine clipping cases", TEST_ENABLED, 0, 0 };

static const TestCaseReference test2 =
		(TestCaseReference){ "rect_testIntersectRectAndLineInside", "Tests SDL_IntersectRectAndLine with line fully contained in rect", TEST_ENABLED, 0, 0 };

static const TestCaseReference test3 =
		(TestCaseReference){ "rect_testIntersectRectAndLineOutside", "Tests SDL_IntersectRectAndLine with line fully outside of rect", TEST_ENABLED, 0, 0 };

static const TestCaseReference test4 =
		(TestCaseReference){ "rect_testIntersectRectAndLineEmpty", "Tests SDL_IntersectRectAndLine with empty rectangle ", TEST_ENABLED, 0, 0 };

static const TestCaseReference test5 =
		(TestCaseReference){ "rect_testIntersectRectAndLineParam", "Negative tests against SDL_IntersectRectAndLine with invalid parameters", TEST_ENABLED, 0, 0 };

/* SDL_IntersectRect */
static const TestCaseReference test6 =
		(TestCaseReference){ "rect_testIntersectRectInside", "Tests SDL_IntersectRect with B fully contained in A", TEST_ENABLED, 0, 0 };

static const TestCaseReference test7 =
		(TestCaseReference){ "rect_testIntersectRectOutside", "Tests SDL_IntersectRect with B fully outside of A", TEST_ENABLED, 0, 0 };

static const TestCaseReference test8 =
		(TestCaseReference){ "rect_testIntersectRectPartial", "Tests SDL_IntersectRect with B partially intersecting A", TEST_ENABLED, 0, 0 };

static const TestCaseReference test9 =
		(TestCaseReference){ "rect_testIntersectRectPoint", "Tests SDL_IntersectRect with 1x1 sized rectangles", TEST_ENABLED, 0, 0 };

static const TestCaseReference test10 =
		(TestCaseReference){ "rect_testIntersectRectEmpty", "Tests SDL_IntersectRect with empty rectangles", TEST_ENABLED, 0, 0 };

static const TestCaseReference test11 =
		(TestCaseReference){ "rect_testIntersectRectParam", "Negative tests against SDL_IntersectRect with invalid parameters", TEST_ENABLED, 0, 0 };

/* SDL_HasIntersection */
static const TestCaseReference test12 =
		(TestCaseReference){ "rect_testHasIntersectionInside", "Tests SDL_HasIntersection with B fully contained in A", TEST_ENABLED, 0, 0 };

static const TestCaseReference test13 =
		(TestCaseReference){ "rect_testHasIntersectionOutside", "Tests SDL_HasIntersection with B fully outside of A", TEST_ENABLED, 0, 0 };

static const TestCaseReference test14 =
		(TestCaseReference){ "rect_testHasIntersectionPartial", "Tests SDL_HasIntersection with B partially intersecting A", TEST_ENABLED, 0, 0 };

static const TestCaseReference test15 =
		(TestCaseReference){ "rect_testHasIntersectionPoint", "Tests SDL_HasIntersection with 1x1 sized rectangles", TEST_ENABLED, 0, 0 };

static const TestCaseReference test16 =
		(TestCaseReference){ "rect_testHasIntersectionEmpty", "Tests SDL_HasIntersection with empty rectangles", TEST_ENABLED, 0, 0 };

static const TestCaseReference test17 =
		(TestCaseReference){ "rect_testHasIntersectionParam", "Negative tests against SDL_HasIntersection with invalid parameters", TEST_ENABLED, 0, 0 };

/* SDL_EnclosePoints */
static const TestCaseReference test18 =
		(TestCaseReference){ "rect_testEnclosePoints", "Tests SDL_EnclosePoints without clipping", TEST_ENABLED, 0, 0 };

static const TestCaseReference test19 =
		(TestCaseReference){ "rect_testEnclosePointsWithClipping", "Tests SDL_EnclosePoints with clipping", TEST_ENABLED, 0, 0 };

static const TestCaseReference test20 =
		(TestCaseReference){ "rect_testEnclosePointsRepeatedInput", "Tests SDL_EnclosePoints with repeated input", TEST_ENABLED, 0, 0 };

static const TestCaseReference test21 =
		(TestCaseReference){ "rect_testEnclosePointsParam", "Negative tests against SDL_EnclosePoints with invalid parameters", TEST_ENABLED, 0, 0 };

/* SDL_UnionRect */
static const TestCaseReference test22 =
		(TestCaseReference){ "rect_testUnionRectInside", "Tests SDL_UnionRect where rect B is inside rect A", TEST_ENABLED, 0, 0 };

static const TestCaseReference test23 =
		(TestCaseReference){ "rect_testUnionRectOutside", "Tests SDL_UnionRect where rect B is outside rect A", TEST_ENABLED, 0, 0 };

static const TestCaseReference test24 =
		(TestCaseReference){ "rect_testUnionRectEmpty", "Tests SDL_UnionRect where rect A or rect B are empty", TEST_ENABLED, 0, 0 };

static const TestCaseReference test25 =
		(TestCaseReference){ "rect_testUnionRectParam", "Negative tests against SDL_UnionRect with invalid parameters", TEST_ENABLED, 0, 0 };

/* SDL_RectEmpty */
static const TestCaseReference test26 =
		(TestCaseReference){ "rect_testRectEmpty", "Tests SDL_RectEmpty with various inputs", TEST_ENABLED, 0, 0 };

static const TestCaseReference test27 =
		(TestCaseReference){ "rect_testRectEmptyParam", "Negative tests against SDL_RectEmpty with invalid parameters", TEST_ENABLED, 0, 0 };

/* SDL_RectEquals */

static const TestCaseReference test28 =
		(TestCaseReference){ "rect_testRectEquals", "Tests SDL_RectEquals with various inputs", TEST_ENABLED, 0, 0 };

static const TestCaseReference test29 =
		(TestCaseReference){ "rect_testRectEqualsParam", "Negative tests against SDL_RectEquals with invalid parameters", TEST_ENABLED, 0, 0 };


/*!
 * \brief Test suite for functions that handle simple rectangles including overlaps and merges.
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/CategoryRect
 */
extern const TestCaseReference *testSuite[] =  {
	&test1, &test2, &test3, &test4, &test5, &test6, &test7, &test8, &test9, &test10, &test11, &test12, &test13, &test14, 
	&test15, &test16, &test17, &test18, &test19, &test20, &test21, &test22, &test23, &test24, &test25, &test26, &test27,
	&test28, &test29, NULL
};

TestCaseReference **QueryTestSuite() {
	return (TestCaseReference **)testSuite;
}

/*!
 * \brief Private helper to check SDL_IntersectRectAndLine results
 */
void _validateIntersectRectAndLineResults(
    SDL_bool intersection, SDL_bool expectedIntersection,
    SDL_Rect *rect, SDL_Rect * refRect,
    int x1, int y1, int x2, int y2,
    int x1Ref, int y1Ref, int x2Ref, int y2Ref)
{
    AssertTrue(intersection == expectedIntersection, 
        "Incorrect intersection result: expected %s, got %s intersecting rect (%d,%d,%d,%d) with line (%d,%d - %d,%d)\n",
        (expectedIntersection == SDL_TRUE) ? "true" : "false",
        (intersection == SDL_TRUE) ? "true" : "false",
        refRect->x, refRect->y, refRect->w, refRect->h,
        x1Ref, y1Ref, x2Ref, y2Ref);
    AssertTrue(rect->x == refRect->x && rect->y == refRect->y && rect->w == refRect->w && rect->h == refRect->h,
        "Source rectangle was modified: got (%d,%d,%d,%d) expected (%d,%d,%d,%d)",
        rect->x, rect->y, rect->w, rect->h,
        refRect->x, refRect->y, refRect->w, refRect->h);
    AssertTrue(x1 == x1Ref && y1 == y1Ref && x2 == x2Ref && y2 == y2Ref,
        "Line was incorrectly clipped or modified: got (%d,%d - %d,%d) expected (%d,%d - %d,%d)",
        x1, y1, x2, y2,
        x1Ref, y1Ref, x2Ref, y2Ref);
}

/*!
 * \brief Tests SDL_IntersectRectAndLine() clipping cases
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_IntersectRectAndLine
 */
int rect_testIntersectRectAndLine (void *arg)
{
    SDL_Rect refRect = { 0, 0, 32, 32 };
    SDL_Rect rect;
    int x1, y1;
    int x2, y2;
    SDL_bool intersected;

    int xLeft = -RandomIntegerInRange(1, refRect.w);
    int xRight = refRect.w + RandomIntegerInRange(1, refRect.w);
    int yTop = -RandomIntegerInRange(1, refRect.h);
    int yBottom = refRect.h + RandomIntegerInRange(1, refRect.h);

    x1 = xLeft;
    y1 = 15;
    x2 = xRight;
    y2 = 15;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_TRUE, &rect, &refRect, x1, y1, x2, y2, 0, 15, 31, 15);

    x1 = 15;
    y1 = yTop;
    x2 = 15;
    y2 = yBottom;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_TRUE, &rect, &refRect, x1, y1, x2, y2, 15, 0, 15, 31);

    x1 = -refRect.w;
    y1 = -refRect.h;
    x2 = 2*refRect.w;
    y2 = 2*refRect.h;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
     _validateIntersectRectAndLineResults(intersected, SDL_TRUE, &rect, &refRect, x1, y1, x2, y2, 0, 0, 31, 31);

    x1 = 2*refRect.w;
    y1 = 2*refRect.h;
    x2 = -refRect.w;
    y2 = -refRect.h;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_TRUE, &rect, &refRect, x1, y1, x2, y2, 31, 31, 0, 0);

    x1 = -1;
    y1 = 32;
    x2 = 32;
    y2 = -1;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_TRUE, &rect, &refRect, x1, y1, x2, y2, 0, 31, 31, 0);

    x1 = 32;
    y1 = -1;
    x2 = -1;
    y2 = 32;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_TRUE, &rect, &refRect, x1, y1, x2, y2, 31, 0, 0, 31);
}

/*!
 * \brief Tests SDL_IntersectRectAndLine() non-clipping case line inside
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_IntersectRectAndLine
 */
int rect_testIntersectRectAndLineInside (void *arg)
{
    SDL_Rect refRect = { 0, 0, 32, 32 };
    SDL_Rect rect;
    int x1, y1;
    int x2, y2;
    SDL_bool intersected;

    int xmin = refRect.x;
    int xmax = refRect.x + refRect.w - 1;
    int ymin = refRect.y;
    int ymax = refRect.y + refRect.h - 1;
    int x1Ref = RandomIntegerInRange(xmin + 1, xmax - 1);
    int y1Ref = RandomIntegerInRange(ymin + 1, ymax - 1);
    int x2Ref = RandomIntegerInRange(xmin + 1, xmax - 1);
    int y2Ref = RandomIntegerInRange(ymin + 1, ymax - 1);

    x1 = x1Ref;
    y1 = y1Ref;
    x2 = x2Ref;
    y2 = y2Ref;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_TRUE, &rect, &refRect, x1, y1, x2, y2, x1Ref, y1Ref, x2Ref, y2Ref);

    x1 = x1Ref;
    y1 = y1Ref;
    x2 = xmax;
    y2 = ymax;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_TRUE, &rect, &refRect, x1, y1, x2, y2, x1Ref, y1Ref, xmax, ymax);

    x1 = xmin;
    y1 = ymin;
    x2 = x2Ref;
    y2 = y2Ref;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_TRUE, &rect, &refRect, x1, y1, x2, y2, xmin, ymin, x2Ref, y2Ref);

    x1 = xmin;
    y1 = ymin;
    x2 = xmax;
    y2 = ymax;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_TRUE, &rect, &refRect, x1, y1, x2, y2, xmin, ymin, xmax, ymax);

    x1 = xmin;
    y1 = ymax;
    x2 = xmax;
    y2 = ymin;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_TRUE, &rect, &refRect, x1, y1, x2, y2, xmin, ymax, xmax, ymin);
}


/*!
 * \brief Tests SDL_IntersectRectAndLine() non-clipping cases outside
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_IntersectRectAndLine
 */
int rect_testIntersectRectAndLineOutside (void *arg)
{
    SDL_Rect refRect = { 0, 0, 32, 32 };
    SDL_Rect rect;
    int x1, y1;
    int x2, y2;
    SDL_bool intersected;

    int xLeft = -RandomIntegerInRange(1, refRect.w);
    int xRight = refRect.w + RandomIntegerInRange(1, refRect.w);
    int yTop = -RandomIntegerInRange(1, refRect.h);
    int yBottom = refRect.h + RandomIntegerInRange(1, refRect.h);

    x1 = xLeft;
    y1 = 0;
    x2 = xLeft;
    y2 = 31;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_FALSE, &rect, &refRect, x1, y1, x2, y2, xLeft, 0, xLeft, 31);

    x1 = xRight;
    y1 = 0;
    x2 = xRight;
    y2 = 31;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_FALSE, &rect, &refRect, x1, y1, x2, y2, xRight, 0, xRight, 31);

    x1 = 0;
    y1 = yTop;
    x2 = 31;
    y2 = yTop;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_FALSE, &rect, &refRect, x1, y1, x2, y2, 0, yTop, 31, yTop);

    x1 = 0;
    y1 = yBottom;
    x2 = 31;
    y2 = yBottom;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_FALSE, &rect, &refRect, x1, y1, x2, y2, 0, yBottom, 31, yBottom);
}

/*!
 * \brief Tests SDL_IntersectRectAndLine() with empty rectangle
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_IntersectRectAndLine
 */
int rect_testIntersectRectAndLineEmpty (void *arg)
{
    SDL_Rect refRect;
    SDL_Rect rect;
    int x1, y1, x1Ref, y1Ref;
    int x2, y2, x2Ref, y2Ref;
    SDL_bool intersected;
        
    refRect.x = RandomIntegerInRange(1, 1024);
    refRect.y = RandomIntegerInRange(1, 1024);
    refRect.w = 0;
    refRect.h = 0;
    x1Ref = refRect.x;
    y1Ref = refRect.y;
    x2Ref = RandomIntegerInRange(1, 1024);
    y2Ref = RandomIntegerInRange(1, 1024);

    x1 = x1Ref;
    y1 = y1Ref;
    x2 = x2Ref;
    y2 = y2Ref;
    rect = refRect;
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    _validateIntersectRectAndLineResults(intersected, SDL_FALSE, &rect, &refRect, x1, y1, x2, y2, x1Ref, y1Ref, x2Ref, y2Ref);
}

/*!
 * \brief Negative tests against SDL_IntersectRectAndLine() with invalid parameters
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_IntersectRectAndLine
 */
int rect_testIntersectRectAndLineParam (void *arg)
{
    SDL_Rect rect = { 0, 0, 32, 32 };
    int x1 = rect.w / 2;
    int y1 = rect.h / 2;
    int x2 = x1;
    int y2 = 2 * rect.h;
    SDL_bool intersected;
    
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2);
    AssertTrue(intersected == SDL_TRUE, "Test variables not intersected as expected");
    
    intersected = SDL_IntersectRectAndLine((SDL_Rect *)NULL, &x1, &y1, &x2, &y2);
    AssertTrue(intersected == SDL_FALSE, "Incorrect intersected result when 1st parameter is NULL");
    intersected = SDL_IntersectRectAndLine(&rect, (int *)NULL, &y1, &x2, &y2);
    AssertTrue(intersected == SDL_FALSE, "Incorrect intersected result when 2nd parameter is NULL");
    intersected = SDL_IntersectRectAndLine(&rect, &x1, (int *)NULL, &x2, &y2);
    AssertTrue(intersected == SDL_FALSE, "Incorrect intersected result when 3rd parameter is NULL");
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, (int *)NULL, &y2);
    AssertTrue(intersected == SDL_FALSE, "Incorrect intersected result when 4th parameter is NULL");
    intersected = SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, (int *)NULL);
    AssertTrue(intersected == SDL_FALSE, "Incorrect intersected result when 5th parameter is NULL");
    intersected = SDL_IntersectRectAndLine((SDL_Rect *)NULL, (int *)NULL, (int *)NULL, (int *)NULL, (int *)NULL);
    AssertTrue(intersected == SDL_FALSE, "Incorrect intersected result when all parameters are NULL");
}

/*!
 * \brief Private helper to check SDL_HasIntersection results
 */
void _validateHasIntersectionResults(
    SDL_bool intersection, SDL_bool expectedIntersection, 
    SDL_Rect *rectA, SDL_Rect *rectB, SDL_Rect *refRectA, SDL_Rect *refRectB)
{
    AssertTrue(intersection == expectedIntersection, 
        "Incorrect intersection result: expected %s, got %s intersecting A (%d,%d,%d,%d) with B (%d,%d,%d,%d)\n",
        (expectedIntersection == SDL_TRUE) ? "true" : "false",
        (intersection == SDL_TRUE) ? "true" : "false",
        rectA->x, rectA->y, rectA->w, rectA->h, 
        rectB->x, rectB->y, rectB->w, rectB->h);
    AssertTrue(rectA->x == refRectA->x && rectA->y == refRectA->y && rectA->w == refRectA->w && rectA->h == refRectA->h,
        "Source rectangle A was modified: got (%d,%d,%d,%d) expected (%d,%d,%d,%d)",
        rectA->x, rectA->y, rectA->w, rectA->h,
        refRectA->x, refRectA->y, refRectA->w, refRectA->h);
    AssertTrue(rectB->x == refRectB->x && rectB->y == refRectB->y && rectB->w == refRectB->w && rectB->h == refRectB->h,
        "Source rectangle B was modified: got (%d,%d,%d,%d) expected (%d,%d,%d,%d)",
        rectB->x, rectB->y, rectB->w, rectB->h,
        refRectB->x, refRectB->y, refRectB->w, refRectB->h);
}

/*!
 * \brief Private helper to check SDL_IntersectRect results
 */
void _validateIntersectRectResults(
    SDL_bool intersection, SDL_bool expectedIntersection, 
    SDL_Rect *rectA, SDL_Rect *rectB, SDL_Rect *refRectA, SDL_Rect *refRectB, 
    SDL_Rect *result, SDL_Rect *expectedResult)
{
    _validateHasIntersectionResults(intersection, expectedIntersection, rectA, rectB, refRectA, refRectB);
    if (result && expectedResult) {
        AssertTrue(result->x == expectedResult->x && result->y == expectedResult->y && result->w == expectedResult->w && result->h == expectedResult->h,
            "Intersection of rectangles A (%d,%d,%d,%d) and B (%d,%d,%d,%d) was incorrectly calculated, got (%d,%d,%d,%d) expected (%d,%d,%d,%d)",
            rectA->x, rectA->y, rectA->w, rectA->h, 
            rectB->x, rectB->y, rectB->w, rectB->h,
            result->x, result->y, result->w, result->h,
            expectedResult->x, expectedResult->y, expectedResult->w, expectedResult->h);
    }
}

/*!
 * \brief Private helper to check SDL_UnionRect results
 */
void _validateUnionRectResults(
    SDL_Rect *rectA, SDL_Rect *rectB, SDL_Rect *refRectA, SDL_Rect *refRectB, 
    SDL_Rect *result, SDL_Rect *expectedResult)
{
    AssertTrue(rectA->x == refRectA->x && rectA->y == refRectA->y && rectA->w == refRectA->w && rectA->h == refRectA->h,
        "Source rectangle A was modified: got (%d,%d,%d,%d) expected (%d,%d,%d,%d)",
        rectA->x, rectA->y, rectA->w, rectA->h,
        refRectA->x, refRectA->y, refRectA->w, refRectA->h);
    AssertTrue(rectB->x == refRectB->x && rectB->y == refRectB->y && rectB->w == refRectB->w && rectB->h == refRectB->h,
        "Source rectangle B was modified: got (%d,%d,%d,%d) expected (%d,%d,%d,%d)",
        rectB->x, rectB->y, rectB->w, rectB->h,
        refRectB->x, refRectB->y, refRectB->w, refRectB->h);
    AssertTrue(result->x == expectedResult->x && result->y == expectedResult->y && result->w == expectedResult->w && result->h == expectedResult->h,
        "Union of rectangles A (%d,%d,%d,%d) and B (%d,%d,%d,%d) was incorrectly calculated, got (%d,%d,%d,%d) expected (%d,%d,%d,%d)",
        rectA->x, rectA->y, rectA->w, rectA->h, 
        rectB->x, rectB->y, rectB->w, rectB->h,
        result->x, result->y, result->w, result->h,
        expectedResult->x, expectedResult->y, expectedResult->w, expectedResult->h);
}

/*!
 * \brief Private helper to check SDL_RectEmpty results
 */
void _validateRectEmptyResults(
    SDL_bool empty, SDL_bool expectedEmpty, 
    SDL_Rect *rect, SDL_Rect *refRect)
{
    AssertTrue(empty == expectedEmpty, 
        "Incorrect empty result: expected %s, got %s testing (%d,%d,%d,%d)\n",
        (expectedEmpty == SDL_TRUE) ? "true" : "false",
        (empty == SDL_TRUE) ? "true" : "false",
        rect->x, rect->y, rect->w, rect->h);
    AssertTrue(rect->x == refRect->x && rect->y == refRect->y && rect->w == refRect->w && rect->h == refRect->h,
        "Source rectangle was modified: got (%d,%d,%d,%d) expected (%d,%d,%d,%d)",
        rect->x, rect->y, rect->w, rect->h,
        refRect->x, refRect->y, refRect->w, refRect->h);
}

/*!
 * \brief Private helper to check SDL_RectEquals results
 */
void _validateRectEqualsResults(
    SDL_bool equals, SDL_bool expectedEquals, 
    SDL_Rect *rectA, SDL_Rect *rectB, SDL_Rect *refRectA, SDL_Rect *refRectB)
{
    AssertTrue(equals == expectedEquals, 
        "Incorrect equals result: expected %s, got %s testing (%d,%d,%d,%d) and (%d,%d,%d,%d) \n",
        (expectedEquals == SDL_TRUE) ? "true" : "false",
        (equals == SDL_TRUE) ? "true" : "false",
        rectA->x, rectA->y, rectA->w, rectA->h,
        rectB->x, rectB->y, rectB->w, rectB->h);
    AssertTrue(rectA->x == refRectA->x && rectA->y == refRectA->y && rectA->w == refRectA->w && rectA->h == refRectA->h,
        "Source rectangle A was modified: got (%d,%d,%d,%d) expected (%d,%d,%d,%d)",
        rectA->x, rectA->y, rectA->w, rectA->h,
        refRectA->x, refRectA->y, refRectA->w, refRectA->h);
    AssertTrue(rectB->x == refRectB->x && rectB->y == refRectB->y && rectB->w == refRectB->w && rectB->h == refRectB->h,
        "Source rectangle B was modified: got (%d,%d,%d,%d) expected (%d,%d,%d,%d)",
        rectB->x, rectB->y, rectB->w, rectB->h,
        refRectB->x, refRectB->y, refRectB->w, refRectB->h);
}

/*!
 * \brief Tests SDL_IntersectRect() with B fully inside A
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_IntersectRect
 */
int rect_testIntersectRectInside (void *arg)
{
    SDL_Rect refRectA = { 0, 0, 32, 32 };
    SDL_Rect refRectB;
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_Rect result;
    SDL_bool intersection;

    // rectB fully contained in rectA
    refRectB.x = 0;
    refRectB.y = 0;
    refRectB.w = RandomIntegerInRange(refRectA.x + 1, refRectA.x + refRectA.w - 1);
    refRectB.h = RandomIntegerInRange(refRectA.y + 1, refRectA.y + refRectA.h - 1);
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_IntersectRect(&rectA, &rectB, &result);
    _validateIntersectRectResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB, &result, &refRectB);
}

/*!
 * \brief Tests SDL_IntersectRect() with B fully outside A
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_IntersectRect
 */
int rect_testIntersectRectOutside (void *arg)
{
    SDL_Rect refRectA = { 0, 0, 32, 32 };
    SDL_Rect refRectB;
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_Rect result;
    SDL_bool intersection;

    // rectB fully outside of rectA
    refRectB.x = refRectA.x + refRectA.w + RandomIntegerInRange(1, 10);
    refRectB.y = refRectA.y + refRectA.h + RandomIntegerInRange(1, 10);
    refRectB.w = refRectA.w;
    refRectB.h = refRectA.h;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_IntersectRect(&rectA, &rectB, &result);
    _validateIntersectRectResults(intersection, SDL_FALSE, &rectA, &rectB, &refRectA, &refRectB, (SDL_Rect *)NULL, (SDL_Rect *)NULL);    
}

/*!
 * \brief Tests SDL_IntersectRect() with B partially intersecting A
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_IntersectRect
 */
int rect_testIntersectRectPartial (void *arg)
{
    SDL_Rect refRectA = { 0, 0, 32, 32 };
    SDL_Rect refRectB;
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_Rect result;
    SDL_Rect expectedResult;
    SDL_bool intersection;

    // rectB partially contained in rectA
    refRectB.x = RandomIntegerInRange(refRectA.x + 1, refRectA.x + refRectA.w - 1);
    refRectB.y = RandomIntegerInRange(refRectA.y + 1, refRectA.y + refRectA.h - 1);
    refRectB.w = refRectA.w;
    refRectB.h = refRectA.h;
    rectA = refRectA;
    rectB = refRectB;
    expectedResult.x = refRectB.x;
    expectedResult.y = refRectB.y;
    expectedResult.w = refRectA.w - refRectB.x;
    expectedResult.h = refRectA.h - refRectB.y;    
    intersection = SDL_IntersectRect(&rectA, &rectB, &result);
    _validateIntersectRectResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);

    // rectB right edge
    refRectB.x = rectA.w - 1;
    refRectB.y = rectA.y;
    refRectB.w = RandomIntegerInRange(1, refRectA.w - 1);
    refRectB.h = RandomIntegerInRange(1, refRectA.h - 1);
    rectA = refRectA;
    rectB = refRectB;
    expectedResult.x = refRectB.x;
    expectedResult.y = refRectB.y;
    expectedResult.w = 1;
    expectedResult.h = refRectB.h;    
    intersection = SDL_IntersectRect(&rectA, &rectB, &result);
    _validateIntersectRectResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);

    // rectB left edge
    refRectB.x = 1 - rectA.w;
    refRectB.y = rectA.y;
    refRectB.w = refRectA.w;
    refRectB.h = RandomIntegerInRange(1, refRectA.h - 1);
    rectA = refRectA;
    rectB = refRectB;
    expectedResult.x = 0;
    expectedResult.y = refRectB.y;
    expectedResult.w = 1;
    expectedResult.h = refRectB.h;    
    intersection = SDL_IntersectRect(&rectA, &rectB, &result);
    _validateIntersectRectResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);

    // rectB bottom edge
    refRectB.x = rectA.x;
    refRectB.y = rectA.h - 1;
    refRectB.w = RandomIntegerInRange(1, refRectA.w - 1);
    refRectB.h = RandomIntegerInRange(1, refRectA.h - 1);
    rectA = refRectA;
    rectB = refRectB;
    expectedResult.x = refRectB.x;
    expectedResult.y = refRectB.y;
    expectedResult.w = refRectB.w;
    expectedResult.h = 1;    
    intersection = SDL_IntersectRect(&rectA, &rectB, &result);
    _validateIntersectRectResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);

    // rectB top edge
    refRectB.x = rectA.x;
    refRectB.y = 1 - rectA.h;
    refRectB.w = RandomIntegerInRange(1, refRectA.w - 1);
    refRectB.h = rectA.h;
    rectA = refRectA;
    rectB = refRectB;
    expectedResult.x = refRectB.x;
    expectedResult.y = 0;
    expectedResult.w = refRectB.w;
    expectedResult.h = 1;    
    intersection = SDL_IntersectRect(&rectA, &rectB, &result);
    _validateIntersectRectResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);
}

/*!
 * \brief Tests SDL_IntersectRect() with 1x1 pixel sized rectangles
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_IntersectRect
 */
int rect_testIntersectRectPoint (void *arg)
{
    SDL_Rect refRectA = { 0, 0, 1, 1 };
    SDL_Rect refRectB = { 0, 0, 1, 1 };
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_Rect result;
    SDL_bool intersection;
    int offsetX, offsetY;

    // intersecting pixels
    refRectA.x = RandomIntegerInRange(1, 100);
    refRectA.y = RandomIntegerInRange(1, 100);
    refRectB.x = refRectA.x;
    refRectB.y = refRectA.y;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_IntersectRect(&rectA, &rectB, &result);
    _validateIntersectRectResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB, &result, &refRectA);

    // non-intersecting pixels cases
    for (offsetX = -1; offsetX <= 1; offsetX++) {
        for (offsetY = -1; offsetY <= 1; offsetY++) {
            if (offsetX != 0 || offsetY != 0) {
                refRectA.x = RandomIntegerInRange(1, 100);
                refRectA.y = RandomIntegerInRange(1, 100);
                refRectB.x = refRectA.x;
                refRectB.y = refRectA.y;    
                refRectB.x += offsetX;
                refRectB.y += offsetY;
                rectA = refRectA;
                rectB = refRectB;
                intersection = SDL_IntersectRect(&rectA, &rectB, &result);
                _validateIntersectRectResults(intersection, SDL_FALSE, &rectA, &rectB, &refRectA, &refRectB, (SDL_Rect *)NULL, (SDL_Rect *)NULL);
            }
        }
    }
}

/*!
 * \brief Tests SDL_IntersectRect() with empty rectangles
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_IntersectRect
 */
int rect_testIntersectRectEmpty (void *arg)
{
    SDL_Rect refRectA;
    SDL_Rect refRectB;
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_Rect result;
    SDL_bool intersection;

    // Rect A empty
    refRectA.x = RandomIntegerInRange(1, 100);
    refRectA.y = RandomIntegerInRange(1, 100);
    refRectA.w = RandomIntegerInRange(1, 100);
    refRectA.h = RandomIntegerInRange(1, 100);
    refRectB = refRectA;
    refRectA.w = 0;
    refRectA.h = 0;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_IntersectRect(&rectA, &rectB, &result);
    _validateIntersectRectResults(intersection, SDL_FALSE, &rectA, &rectB, &refRectA, &refRectB, (SDL_Rect *)NULL, (SDL_Rect *)NULL);

    // Rect B empty
    refRectA.x = RandomIntegerInRange(1, 100);
    refRectA.y = RandomIntegerInRange(1, 100);
    refRectA.w = RandomIntegerInRange(1, 100);
    refRectA.h = RandomIntegerInRange(1, 100);
    refRectB = refRectA;
    refRectB.w = 0;
    refRectB.h = 0;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_IntersectRect(&rectA, &rectB, &result);
    _validateIntersectRectResults(intersection, SDL_FALSE, &rectA, &rectB, &refRectA, &refRectB, (SDL_Rect *)NULL, (SDL_Rect *)NULL);

    // Rect A and B empty
    refRectA.x = RandomIntegerInRange(1, 100);
    refRectA.y = RandomIntegerInRange(1, 100);
    refRectA.w = RandomIntegerInRange(1, 100);
    refRectA.h = RandomIntegerInRange(1, 100);
    refRectB = refRectA;
    refRectA.w = 0;
    refRectA.h = 0;
    refRectB.w = 0;
    refRectB.h = 0;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_IntersectRect(&rectA, &rectB, &result);
    _validateIntersectRectResults(intersection, SDL_FALSE, &rectA, &rectB, &refRectA, &refRectB, (SDL_Rect *)NULL, (SDL_Rect *)NULL);
}

/*!
 * \brief Negative tests against SDL_IntersectRect() with invalid parameters
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_IntersectRect
 */
int rect_testIntersectRectParam(void *arg)
{
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_Rect result;
    SDL_bool intersection;

    // invalid parameter combinations
    intersection = SDL_IntersectRect((SDL_Rect *)NULL, &rectB, &result);
    AssertTrue(intersection == SDL_FALSE, "Function did not return false when 1st parameter was NULL"); 
    intersection = SDL_IntersectRect(&rectA, (SDL_Rect *)NULL, &result);
    AssertTrue(intersection == SDL_FALSE, "Function did not return false when 2st parameter was NULL"); 
    intersection = SDL_IntersectRect(&rectA, &rectB, (SDL_Rect *)NULL);
    AssertTrue(intersection == SDL_FALSE, "Function did not return false when 3st parameter was NULL"); 
    intersection = SDL_IntersectRect((SDL_Rect *)NULL, (SDL_Rect *)NULL, &result);
    AssertTrue(intersection == SDL_FALSE, "Function did not return false when 1st and 2nd parameters were NULL"); 
    intersection = SDL_IntersectRect((SDL_Rect *)NULL, &rectB, (SDL_Rect *)NULL);
    AssertTrue(intersection == SDL_FALSE, "Function did not return false when 1st and 3rd parameters were NULL "); 
    intersection = SDL_IntersectRect((SDL_Rect *)NULL, (SDL_Rect *)NULL, (SDL_Rect *)NULL);
    AssertTrue(intersection == SDL_FALSE, "Function did not return false when all parameters were NULL");     
}

/*!
 * \brief Tests SDL_HasIntersection() with B fully inside A
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_HasIntersection
 */
int rect_testHasIntersectionInside (void *arg)
{
    SDL_Rect refRectA = { 0, 0, 32, 32 };
    SDL_Rect refRectB;
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_bool intersection;

    // rectB fully contained in rectA
    refRectB.x = 0;
    refRectB.y = 0;
    refRectB.w = RandomIntegerInRange(refRectA.x + 1, refRectA.x + refRectA.w - 1);
    refRectB.h = RandomIntegerInRange(refRectA.y + 1, refRectA.y + refRectA.h - 1);
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_HasIntersection(&rectA, &rectB);
    _validateHasIntersectionResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB);
}

/*!
 * \brief Tests SDL_HasIntersection() with B fully outside A
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_HasIntersection
 */
int rect_testHasIntersectionOutside (void *arg)
{
    SDL_Rect refRectA = { 0, 0, 32, 32 };
    SDL_Rect refRectB;
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_bool intersection;

    // rectB fully outside of rectA
    refRectB.x = refRectA.x + refRectA.w + RandomIntegerInRange(1, 10);
    refRectB.y = refRectA.y + refRectA.h + RandomIntegerInRange(1, 10);
    refRectB.w = refRectA.w;
    refRectB.h = refRectA.h;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_HasIntersection(&rectA, &rectB);
    _validateHasIntersectionResults(intersection, SDL_FALSE, &rectA, &rectB, &refRectA, &refRectB);
}

/*!
 * \brief Tests SDL_HasIntersection() with B partially intersecting A
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_HasIntersection
 */
int rect_testHasIntersectionPartial (void *arg)
{
    SDL_Rect refRectA = { 0, 0, 32, 32 };
    SDL_Rect refRectB;
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_bool intersection;

    // rectB partially contained in rectA
    refRectB.x = RandomIntegerInRange(refRectA.x + 1, refRectA.x + refRectA.w - 1);
    refRectB.y = RandomIntegerInRange(refRectA.y + 1, refRectA.y + refRectA.h - 1);
    refRectB.w = refRectA.w;
    refRectB.h = refRectA.h;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_HasIntersection(&rectA, &rectB);
    _validateHasIntersectionResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB);

    // rectB right edge
    refRectB.x = rectA.w - 1;
    refRectB.y = rectA.y;
    refRectB.w = RandomIntegerInRange(1, refRectA.w - 1);
    refRectB.h = RandomIntegerInRange(1, refRectA.h - 1);
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_HasIntersection(&rectA, &rectB);
    _validateHasIntersectionResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB);

    // rectB left edge
    refRectB.x = 1 - rectA.w;
    refRectB.y = rectA.y;
    refRectB.w = refRectA.w;
    refRectB.h = RandomIntegerInRange(1, refRectA.h - 1);
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_HasIntersection(&rectA, &rectB);
    _validateHasIntersectionResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB);

    // rectB bottom edge
    refRectB.x = rectA.x;
    refRectB.y = rectA.h - 1;
    refRectB.w = RandomIntegerInRange(1, refRectA.w - 1);
    refRectB.h = RandomIntegerInRange(1, refRectA.h - 1);
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_HasIntersection(&rectA, &rectB);
    _validateHasIntersectionResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB);

    // rectB top edge
    refRectB.x = rectA.x;
    refRectB.y = 1 - rectA.h;
    refRectB.w = RandomIntegerInRange(1, refRectA.w - 1);
    refRectB.h = rectA.h;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_HasIntersection(&rectA, &rectB);
    _validateHasIntersectionResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB);
}

/*!
 * \brief Tests SDL_HasIntersection() with 1x1 pixel sized rectangles
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_HasIntersection
 */
int rect_testHasIntersectionPoint (void *arg)
{
    SDL_Rect refRectA = { 0, 0, 1, 1 };
    SDL_Rect refRectB = { 0, 0, 1, 1 };
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_Rect result;
    SDL_bool intersection;
    int offsetX, offsetY;

    // intersecting pixels
    refRectA.x = RandomIntegerInRange(1, 100);
    refRectA.y = RandomIntegerInRange(1, 100);
    refRectB.x = refRectA.x;
    refRectB.y = refRectA.y;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_HasIntersection(&rectA, &rectB);
    _validateHasIntersectionResults(intersection, SDL_TRUE, &rectA, &rectB, &refRectA, &refRectB);

    // non-intersecting pixels cases
    for (offsetX = -1; offsetX <= 1; offsetX++) {
        for (offsetY = -1; offsetY <= 1; offsetY++) {
            if (offsetX != 0 || offsetY != 0) {
                refRectA.x = RandomIntegerInRange(1, 100);
                refRectA.y = RandomIntegerInRange(1, 100);
                refRectB.x = refRectA.x;
                refRectB.y = refRectA.y;    
                refRectB.x += offsetX;
                refRectB.y += offsetY;
                rectA = refRectA;
                rectB = refRectB;
                intersection = SDL_HasIntersection(&rectA, &rectB);
                _validateHasIntersectionResults(intersection, SDL_FALSE, &rectA, &rectB, &refRectA, &refRectB);
            }
        }
    }
}

/*!
 * \brief Tests SDL_HasIntersection() with empty rectangles
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_HasIntersection
 */
int rect_testHasIntersectionEmpty (void *arg)
{
    SDL_Rect refRectA;
    SDL_Rect refRectB;
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_bool intersection;

    // Rect A empty
    refRectA.x = RandomIntegerInRange(1, 100);
    refRectA.y = RandomIntegerInRange(1, 100);
    refRectA.w = RandomIntegerInRange(1, 100);
    refRectA.h = RandomIntegerInRange(1, 100);
    refRectB = refRectA;
    refRectA.w = 0;
    refRectA.h = 0;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_HasIntersection(&rectA, &rectB);
    _validateHasIntersectionResults(intersection, SDL_FALSE, &rectA, &rectB, &refRectA, &refRectB);

    // Rect B empty
    refRectA.x = RandomIntegerInRange(1, 100);
    refRectA.y = RandomIntegerInRange(1, 100);
    refRectA.w = RandomIntegerInRange(1, 100);
    refRectA.h = RandomIntegerInRange(1, 100);
    refRectB = refRectA;
    refRectB.w = 0;
    refRectB.h = 0;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_HasIntersection(&rectA, &rectB);
    _validateHasIntersectionResults(intersection, SDL_FALSE, &rectA, &rectB, &refRectA, &refRectB);

    // Rect A and B empty
    refRectA.x = RandomIntegerInRange(1, 100);
    refRectA.y = RandomIntegerInRange(1, 100);
    refRectA.w = RandomIntegerInRange(1, 100);
    refRectA.h = RandomIntegerInRange(1, 100);
    refRectB = refRectA;
    refRectA.w = 0;
    refRectA.h = 0;
    refRectB.w = 0;
    refRectB.h = 0;
    rectA = refRectA;
    rectB = refRectB;
    intersection = SDL_HasIntersection(&rectA, &rectB);
    _validateHasIntersectionResults(intersection, SDL_FALSE, &rectA, &rectB, &refRectA, &refRectB);
}

/*!
 * \brief Negative tests against SDL_HasIntersection() with invalid parameters
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_HasIntersection
 */
int rect_testHasIntersectionParam(void *arg)
{
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_bool intersection;

    // invalid parameter combinations
    intersection = SDL_HasIntersection((SDL_Rect *)NULL, &rectB);
    AssertTrue(intersection == SDL_FALSE, "Function did not return false when 1st parameter was NULL"); 
    intersection = SDL_HasIntersection(&rectA, (SDL_Rect *)NULL);
    AssertTrue(intersection == SDL_FALSE, "Function did not return false when 2st parameter was NULL"); 
    intersection = SDL_HasIntersection((SDL_Rect *)NULL, (SDL_Rect *)NULL);
    AssertTrue(intersection == SDL_FALSE, "Function did not return false when all parameters were NULL");     
}

/*!
 * \brief Test SDL_EnclosePoints() without clipping
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_EnclosePoints
 */
int rect_testEnclosePoints(void *arg)
{
    const int numPoints = 16;
    SDL_Point refPoints[numPoints];
    SDL_Point points[numPoints];
    int count;
    SDL_Rect result;
    SDL_bool anyEnclosed;
    SDL_bool anyEnclosedNoResult;

    // Create input data, tracking result
    SDL_bool expectedEnclosed = SDL_TRUE;
    int newx, newy;
    int minx, maxx, miny, maxy;
    int i;
    for (i=0; i<numPoints; i++) {
        newx = RandomIntegerInRange(-1024, 1024);
        newy = RandomIntegerInRange(-1024, 1024);
        refPoints[i].x = newx;
        refPoints[i].y = newy;
        points[i].x = newx;
        points[i].y = newy;
        if (i==0) {
            minx=maxx=newx;
            miny=maxy=newy;
        } else {
            if (newx<minx) minx=newx;
            if (newx>maxx) maxx=newx;
            if (newy<miny) miny=newy;
            if (newy>maxy) maxy=newy;
        }
    }
    
    // Call function and validate - special case: no result requested
    anyEnclosedNoResult = SDL_EnclosePoints((const SDL_Point *)points, numPoints, (const SDL_Rect *)NULL, (SDL_Rect *)NULL);
    AssertTrue(expectedEnclosed==anyEnclosedNoResult, 
        "Expected return value %s, got %s", 
        (expectedEnclosed==SDL_TRUE) ? "true" : "false", (anyEnclosedNoResult==SDL_TRUE) ? "true" : "false");
    for (i=0; i<numPoints; i++) {
        AssertTrue(refPoints[i].x==points[i].x && refPoints[i].y==points[i].y,
            "Source point %i was incorrectly modified: expected (%i,%i) actual (%i,%i)", 
            i, refPoints[i].x, refPoints[i].y, points[i].x, points[i].y);
    }

    // Call function and validate
    anyEnclosed = SDL_EnclosePoints((const SDL_Point *)points, numPoints, (const SDL_Rect *)NULL, &result);
    AssertTrue(expectedEnclosed==anyEnclosed, 
        "Expected return value %s, got %s", 
        (expectedEnclosed==SDL_TRUE) ? "true" : "false", (anyEnclosed==SDL_TRUE) ? "true" : "false");
    for (i=0; i<numPoints; i++) {
        AssertTrue(refPoints[i].x==points[i].x && refPoints[i].y==points[i].y,
            "Source point %i was incorrectly modified: expected (%i,%i) actual (%i,%i)", 
            i, refPoints[i].x, refPoints[i].y, points[i].x, points[i].y);
    }
    AssertTrue(result.x==minx && result.y==miny && result.w==(maxx - minx + 1) && result.h==(maxy - miny + 1),
        "Resulting enclosing rectangle incorrect: expected (%i,%i - %i,%i), actual (%i,%i - %i,%i)",
        minx, miny, maxx, maxy, result.x, result.y, result.x + result.w - 1, result.y + result.h - 1);
}

/*!
 * \brief Test SDL_EnclosePoints() with repeated input points
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_EnclosePoints
 */
int rect_testEnclosePointsRepeatedInput(void *arg)
{
    const int numPoints = 8;
    const int halfPoints = numPoints/2;
    SDL_Point refPoints[numPoints];
    SDL_Point points[numPoints];
    int count;
    SDL_Rect result;
    SDL_bool anyEnclosed;
    SDL_bool anyEnclosedNoResult;

    // Create input data, tracking result
    SDL_bool expectedEnclosed = SDL_TRUE;
    int newx, newy;
    int minx, maxx, miny, maxy;
    int i;
    for (i=0; i<numPoints; i++) {
        if (i < halfPoints) {
            newx = RandomIntegerInRange(-1024, 1024);
            newy = RandomIntegerInRange(-1024, 1024);
        } else {
            newx = refPoints[i-halfPoints].x;
            newy = refPoints[i-halfPoints].y;
        }
        refPoints[i].x = newx;
        refPoints[i].y = newy;
        points[i].x = newx;
        points[i].y = newy;
        if (i==0) {
            minx=maxx=newx;
            miny=maxy=newy;
        } else {
            if (newx<minx) minx=newx;
            if (newx>maxx) maxx=newx;
            if (newy<miny) miny=newy;
            if (newy>maxy) maxy=newy;
        }
    }

    // Call function and validate - special case: no result requested
    anyEnclosedNoResult = SDL_EnclosePoints((const SDL_Point *)points, numPoints, (const SDL_Rect *)NULL, (SDL_Rect *)NULL);
    AssertTrue(expectedEnclosed==anyEnclosedNoResult, 
        "Expected return value %s, got %s", 
        (expectedEnclosed==SDL_TRUE) ? "true" : "false", (anyEnclosedNoResult==SDL_TRUE) ? "true" : "false");
    for (i=0; i<numPoints; i++) {
        AssertTrue(refPoints[i].x==points[i].x && refPoints[i].y==points[i].y,
            "Source point %i was incorrectly modified: expected (%i,%i) actual (%i,%i)", 
            i, refPoints[i].x, refPoints[i].y, points[i].x, points[i].y);
    }
    
    // Call function and validate
    anyEnclosed = SDL_EnclosePoints((const SDL_Point *)points, numPoints, (const SDL_Rect *)NULL, &result);
    AssertTrue(expectedEnclosed==anyEnclosed, 
        "Expected return value %s, got %s", 
        (expectedEnclosed==SDL_TRUE) ? "true" : "false", (anyEnclosed==SDL_TRUE) ? "true" : "false");
    for (i=0; i<numPoints; i++) {
        AssertTrue(refPoints[i].x==points[i].x && refPoints[i].y==points[i].y,
            "Source point %i was incorrectly modified: expected (%i,%i) actual (%i,%i)", 
            i, refPoints[i].x, refPoints[i].y, points[i].x, points[i].y);
    }
    AssertTrue(result.x==minx && result.y==miny && result.w==(maxx - minx + 1) && result.h==(maxy - miny + 1),
        "Resulting enclosing rectangle incorrect: expected (%i,%i - %i,%i), actual (%i,%i - %i,%i)",
        minx, miny, maxx, maxy, result.x, result.y, result.x + result.w - 1, result.y + result.h - 1);
}

/*!
 * \brief Test SDL_EnclosePoints() with clipping
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_EnclosePoints
 */
int rect_testEnclosePointsWithClipping(void *arg)
{
    const int numPoints = 16;
    SDL_Point refPoints[numPoints];
    SDL_Point points[numPoints];
    int count;
    SDL_Rect refClip;
    SDL_Rect clip;
    SDL_Rect result;
    SDL_bool anyEnclosed;
    SDL_bool anyEnclosedNoResult;

    // Setup clipping rectangle
    refClip.x = RandomIntegerInRange(-1024, 1024);
    refClip.y = RandomIntegerInRange(-1024, 1024);
    refClip.w = RandomIntegerInRange(1, 1024);
    refClip.h = RandomIntegerInRange(1, 1024);

    // Create input data, tracking result
    SDL_bool expectedEnclosed = SDL_FALSE;
    int newx, newy;
    int minx, maxx, miny, maxy;
    int i;
    for (i=0; i<numPoints; i++) {
        newx = RandomIntegerInRange(-1024, 1024);
        newy = RandomIntegerInRange(-1024, 1024);
        refPoints[i].x = newx;
        refPoints[i].y = newy;
        points[i].x = newx;
        points[i].y = newy;
        if ((newx>=refClip.x) && (newx<(refClip.x + refClip.w)) &&
            (newy>=refClip.y) && (newy<(refClip.y + refClip.h))) {
            if (expectedEnclosed==SDL_FALSE) {
                minx=maxx=newx;
                miny=maxy=newy;
            } else {
                if (newx<minx) minx=newx;
                if (newx>maxx) maxx=newx;
                if (newy<miny) miny=newy;
                if (newy>maxy) maxy=newy;
            }
            expectedEnclosed = SDL_TRUE;
        }
    }

    // Call function and validate - special case: no result requested
    clip = refClip;
    anyEnclosedNoResult = SDL_EnclosePoints((const SDL_Point *)points, numPoints, (const SDL_Rect *)&clip, (SDL_Rect *)NULL);
    AssertTrue(expectedEnclosed==anyEnclosedNoResult, 
        "Expected return value %s, got %s", 
        (expectedEnclosed==SDL_TRUE) ? "true" : "false", (anyEnclosedNoResult==SDL_TRUE) ? "true" : "false");
    for (i=0; i<numPoints; i++) {
        AssertTrue(refPoints[i].x==points[i].x && refPoints[i].y==points[i].y,
            "Source point %i was incorrectly modified: expected (%i,%i) actual (%i,%i)", 
            i, refPoints[i].x, refPoints[i].y, points[i].x, points[i].y);
    }
    AssertTrue(refClip.x==clip.x && refClip.y==clip.y && refClip.w==clip.w && refClip.h==clip.h,
        "Source clipping rectangle was incorrectly modified");
    
    // Call function and validate
    anyEnclosed = SDL_EnclosePoints((const SDL_Point *)points, numPoints, (const SDL_Rect *)&clip, &result);
    AssertTrue(expectedEnclosed==anyEnclosed, 
        "Expected return value %s, got %s", 
        (expectedEnclosed==SDL_TRUE) ? "true" : "false", (anyEnclosed==SDL_TRUE) ? "true" : "false");
    for (i=0; i<numPoints; i++) {
        AssertTrue(refPoints[i].x==points[i].x && refPoints[i].y==points[i].y,
            "Source point %i was incorrectly modified: expected (%i,%i) actual (%i,%i)", 
            i, refPoints[i].x, refPoints[i].y, points[i].x, points[i].y);
    }
    AssertTrue(refClip.x==clip.x && refClip.y==clip.y && refClip.w==clip.w && refClip.h==clip.h,
        "Source clipping rectangle was incorrectly modified");
    if (expectedEnclosed==SDL_TRUE) {
        AssertTrue(result.x==minx && result.y==miny && result.w==(maxx - minx + 1) && result.h==(maxy - miny + 1),
            "Resulting enclosing rectangle incorrect: expected (%i,%i - %i,%i), actual (%i,%i - %i,%i)",
            minx, miny, maxx, maxy, result.x, result.y, result.x + result.w - 1, result.y + result.h - 1);
    }    
    
    /* Empty clipping rectangle */
    clip.w = 0;
    clip.h = 0;
    expectedEnclosed = SDL_FALSE;
    anyEnclosed = SDL_EnclosePoints((const SDL_Point *)points, numPoints, (const SDL_Rect *)&clip, &result);
    AssertTrue(expectedEnclosed==anyEnclosed, 
        "Expected return value %s, got %s", 
        (expectedEnclosed==SDL_TRUE) ? "true" : "false", (anyEnclosed==SDL_TRUE) ? "true" : "false");    
}

/*!
 * \brief Negative tests against SDL_EnclosePoints() with invalid parameters
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_EnclosePoints
 */
int rect_testEnclosePointsParam(void *arg)
{
    SDL_Point points[1];
    int count;
    SDL_Rect clip;
    SDL_Rect result;
    SDL_bool anyEnclosed;

    // invalid parameter combinations
    anyEnclosed = SDL_EnclosePoints((SDL_Point *)NULL, 1, (const SDL_Rect *)&clip, &result);
    AssertTrue(anyEnclosed == SDL_FALSE, "Function did not return false when 1st parameter was NULL"); 
    anyEnclosed = SDL_EnclosePoints((const SDL_Point *)points, 0, (const SDL_Rect *)&clip, &result);
    AssertTrue(anyEnclosed == SDL_FALSE, "Function did not return false when 2nd parameter was 0");
    count = RandomIntegerInRange(-100, -1);
    anyEnclosed = SDL_EnclosePoints((const SDL_Point *)points, count, (const SDL_Rect *)&clip, &result);
    AssertTrue(anyEnclosed == SDL_FALSE, "Function did not return false when 2nd parameter was %i", count); 
    anyEnclosed = SDL_EnclosePoints((SDL_Point *)NULL, 0, (const SDL_Rect *)&clip, &result);
    AssertTrue(anyEnclosed == SDL_FALSE, "Function did not return false when 1st parameter was NULL and 2nd parameter was 0"); 
}


/*!
 * \brief Tests SDL_UnionRect() where rect B is outside rect A
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_UnionRect
 */
int rect_testUnionRectOutside(void *arg)
{
    SDL_Rect refRectA, refRectB;
    SDL_Rect rectA, rectB;
    SDL_Rect expectedResult;
    SDL_Rect result;
    int minx, maxx, miny, maxy;
    int dx, dy;
    
    /* Union 1x1 outside */
    for (dx = -1; dx < 2; dx++) {     
        for (dy = -1; dy < 2; dy++) {
            if ((dx != 0) || (dy != 0)) {
                refRectA.x=RandomIntegerInRange(-1024, 1024);
                refRectA.y=RandomIntegerInRange(-1024, 1024);
                refRectA.w=1;
                refRectA.h=1;
                refRectB.x=RandomIntegerInRange(-1024, 1024) + dx*2048;
                refRectB.y=RandomIntegerInRange(-1024, 1024) + dx*2048;
                refRectB.w=1;
                refRectB.h=1;
                minx = (refRectA.x<refRectB.x) ? refRectA.x : refRectB.x;
                maxx = (refRectA.x>refRectB.x) ? refRectA.x : refRectB.x;
                miny = (refRectA.y<refRectB.y) ? refRectA.y : refRectB.y;
                maxy = (refRectA.y>refRectB.y) ? refRectA.y : refRectB.y;                
                expectedResult.x = minx;
                expectedResult.y = miny;
                expectedResult.w = maxx - minx + 1;
                expectedResult.h = maxy - miny + 1;
                rectA = refRectA;
                rectB = refRectB;
                SDL_UnionRect(&rectA, &rectB, &result);
                _validateUnionRectResults(&rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);
            }
        }
    }

    /* Union outside overlap */
    for (dx = -1; dx < 2; dx++) {     
        for (dy = -1; dy < 2; dy++) {
            if ((dx != 0) || (dy != 0)) {
                refRectA.x=RandomIntegerInRange(-1024, 1024);
                refRectA.y=RandomIntegerInRange(-1024, 1024);
                refRectA.w=RandomIntegerInRange(256, 512);
                refRectA.h=RandomIntegerInRange(256, 512);
                refRectB.x=refRectA.x + 1 + dx*2;
                refRectB.y=refRectA.y + 1 + dy*2;
                refRectB.w=refRectA.w - 2;
                refRectB.h=refRectA.h - 2;
                expectedResult = refRectA;
                if (dx == -1) expectedResult.x--;
                if (dy == -1) expectedResult.y--;
                if ((dx == 1) || (dx == -1)) expectedResult.w++;
                if ((dy == 1) || (dy == -1)) expectedResult.h++;
                rectA = refRectA;
                rectB = refRectB;
                SDL_UnionRect(&rectA, &rectB, &result);
                _validateUnionRectResults(&rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);
            }
        }
    }
}

/*!
 * \brief Tests SDL_UnionRect() where rect A or rect B are empty
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_UnionRect
 */
int rect_testUnionRectEmpty(void *arg)
{
    SDL_Rect refRectA, refRectB;
    SDL_Rect rectA, rectB;
    SDL_Rect expectedResult;
    SDL_Rect result;

    /* A empty */    
    refRectA.x=RandomIntegerInRange(-1024, 1024);
    refRectA.y=RandomIntegerInRange(-1024, 1024);
    refRectA.w=0;
    refRectA.h=0;
    refRectB.x=RandomIntegerInRange(-1024, 1024);
    refRectB.y=RandomIntegerInRange(-1024, 1024);
    refRectB.w=RandomIntegerInRange(1, 1024);
    refRectB.h=RandomIntegerInRange(1, 1024);
    expectedResult = refRectB;
    rectA = refRectA;
    rectB = refRectB;
    SDL_UnionRect(&rectA, &rectB, &result);
    _validateUnionRectResults(&rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);
    
    /* B empty */    
    refRectA.x=RandomIntegerInRange(-1024, 1024);
    refRectA.y=RandomIntegerInRange(-1024, 1024);
    refRectA.w=RandomIntegerInRange(1, 1024);
    refRectA.h=RandomIntegerInRange(1, 1024);
    refRectB.x=RandomIntegerInRange(-1024, 1024);
    refRectB.y=RandomIntegerInRange(-1024, 1024);
    refRectB.w=0;
    refRectB.h=0;
    expectedResult = refRectA;
    rectA = refRectA;
    rectB = refRectB;
    SDL_UnionRect(&rectA, &rectB, &result);
    _validateUnionRectResults(&rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);

    /* A and B empty */    
    refRectA.x=RandomIntegerInRange(-1024, 1024);
    refRectA.y=RandomIntegerInRange(-1024, 1024);
    refRectA.w=0;
    refRectA.h=0;
    refRectB.x=RandomIntegerInRange(-1024, 1024);
    refRectB.y=RandomIntegerInRange(-1024, 1024);
    refRectB.w=0;
    refRectB.h=0;
    result.x=0;
    result.y=0;
    result.w=0;
    result.h=0;
    expectedResult = result;
    rectA = refRectA;
    rectB = refRectB;
    SDL_UnionRect(&rectA, &rectB, &result);
    _validateUnionRectResults(&rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);
}

/*!
 * \brief Tests SDL_UnionRect() where rect B is inside rect A
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_UnionRect
 */
int rect_testUnionRectInside(void *arg)
{
    SDL_Rect refRectA, refRectB;
    SDL_Rect rectA, rectB;
    SDL_Rect expectedResult;
    SDL_Rect result;
    int minx, maxx, miny, maxy;
    int dx, dy;
    
    /* Union 1x1 with itself */
    refRectA.x=RandomIntegerInRange(-1024, 1024);
    refRectA.y=RandomIntegerInRange(-1024, 1024);
    refRectA.w=1;
    refRectA.h=1;
    expectedResult = refRectA;
    rectA = refRectA;
    SDL_UnionRect(&rectA, &rectA, &result);
    _validateUnionRectResults(&rectA, &rectA, &refRectA, &refRectA, &result, &expectedResult);

    /* Union 1x1 somewhere inside */
    refRectA.x=RandomIntegerInRange(-1024, 1024);
    refRectA.y=RandomIntegerInRange(-1024, 1024);
    refRectA.w=RandomIntegerInRange(256, 1024);
    refRectA.h=RandomIntegerInRange(256, 1024);
    refRectB.x=refRectA.x + 1 + RandomIntegerInRange(1, refRectA.w - 2);
    refRectB.y=refRectA.y + 1 + RandomIntegerInRange(1, refRectA.h - 2);
    refRectB.w=1;
    refRectB.h=1;
    expectedResult = refRectA;
    rectA = refRectA;
    rectB = refRectB;
    SDL_UnionRect(&rectA, &rectB, &result);
    _validateUnionRectResults(&rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);

    /* Union inside with edges modified */
    for (dx = -1; dx < 2; dx++) {     
        for (dy = -1; dy < 2; dy++) {
            if ((dx != 0) || (dy != 0)) {
                refRectA.x=RandomIntegerInRange(-1024, 1024);
                refRectA.y=RandomIntegerInRange(-1024, 1024);
                refRectA.w=RandomIntegerInRange(256, 1024);
                refRectA.h=RandomIntegerInRange(256, 1024);
                refRectB = refRectA;
                if (dx == -1) refRectB.x++;
                if ((dx == 1) || (dx == -1)) refRectB.w--;
                if (dy == -1) refRectB.y++;
                if ((dy == 1) || (dy == -1)) refRectB.h--;                
                expectedResult = refRectA;
                rectA = refRectA;
                rectB = refRectB;
                SDL_UnionRect(&rectA, &rectB, &result);
                _validateUnionRectResults(&rectA, &rectB, &refRectA, &refRectB, &result, &expectedResult);
            }
        }
    }
}

/*!
 * \brief Negative tests against SDL_UnionRect() with invalid parameters
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_UnionRect
 */
int rect_testUnionRectParam(void *arg)
{
    SDL_Rect rectA, rectB;
    SDL_Rect result;

    // invalid parameter combinations
    SDL_UnionRect((SDL_Rect *)NULL, &rectB, &result);
    AssertPass("Function did return when 1st parameter was NULL"); 
    SDL_UnionRect(&rectA, (SDL_Rect *)NULL, &result);
    AssertPass("Function did return when 2nd parameter was NULL"); 
    SDL_UnionRect(&rectA, &rectB, (SDL_Rect *)NULL);
    AssertPass("Function did return when 3rd parameter was NULL"); 
    SDL_UnionRect((SDL_Rect *)NULL, &rectB, (SDL_Rect *)NULL);
    AssertPass("Function did return when 1st and 3rd parameter were NULL"); 
    SDL_UnionRect(&rectA, (SDL_Rect *)NULL, (SDL_Rect *)NULL);
    AssertPass("Function did return when 2nd and 3rd parameter were NULL"); 
    SDL_UnionRect((SDL_Rect *)NULL, (SDL_Rect *)NULL, (SDL_Rect *)NULL);
    AssertPass("Function did return when all parameters were NULL"); 
}

/*!
 * \brief Tests SDL_RectEmpty() with various inputs
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_RectEmpty
 */
int rect_testRectEmpty(void *arg)
{
    SDL_Rect refRect;
    SDL_Rect rect;
    SDL_bool expectedResult;
    SDL_bool result;
    int w, h;

    // Non-empty case
    refRect.x=RandomIntegerInRange(-1024, 1024);
    refRect.y=RandomIntegerInRange(-1024, 1024);
    refRect.w=RandomIntegerInRange(256, 1024);
    refRect.h=RandomIntegerInRange(256, 1024);
    expectedResult = SDL_FALSE;
    rect = refRect;
    result = SDL_RectEmpty((const SDL_Rect *)&rect);
    _validateRectEmptyResults(result, expectedResult, &rect, &refRect);
    
    // Empty case
    for (w=-1; w<2; w++) {
        for (h=-1; h<2; h++) {
            if ((w != 1) || (h != 1)) {
                refRect.x=RandomIntegerInRange(-1024, 1024);
                refRect.y=RandomIntegerInRange(-1024, 1024);
                refRect.w=w;
                refRect.h=h;
                expectedResult = SDL_TRUE;
                rect = refRect;
                result = SDL_RectEmpty((const SDL_Rect *)&rect);
                _validateRectEmptyResults(result, expectedResult, &rect, &refRect);
            }
        }
    }
}

/*!
 * \brief Negative tests against SDL_RectEmpty() with invalid parameters
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_RectEmpty
 */
int rect_testRectEmptyParam(void *arg)
{
    SDL_bool result;

    // invalid parameter combinations
    result = SDL_RectEmpty((const SDL_Rect *)NULL);
    AssertTrue(result == SDL_TRUE, "Function did not return TRUE when 1st parameter was NULL"); 
}

/*!
 * \brief Tests SDL_RectEquals() with various inputs
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_RectEquals
 */
int rect_testRectEquals(void *arg)
{
    SDL_Rect refRectA;
    SDL_Rect refRectB;
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_bool expectedResult;
    SDL_bool result;

    // Equals
    refRectA.x=RandomIntegerInRange(-1024, 1024);
    refRectA.y=RandomIntegerInRange(-1024, 1024);
    refRectA.w=RandomIntegerInRange(1, 1024);
    refRectA.h=RandomIntegerInRange(1, 1024);
    refRectB = refRectA;    
    expectedResult = SDL_TRUE;
    rectA = refRectA;
    rectB = refRectB;
    result = SDL_RectEquals((const SDL_Rect *)&rectA, (const SDL_Rect *)&rectB);
    _validateRectEqualsResults(result, expectedResult, &rectA, &rectB, &refRectA, &refRectB);
}

/*!
 * \brief Negative tests against SDL_RectEquals() with invalid parameters
 *
 * \sa
 * http://wiki.libsdl.org/moin.cgi/SDL_RectEquals
 */
int rect_testRectEqualsParam(void *arg)
{
    SDL_Rect rectA;
    SDL_Rect rectB;
    SDL_bool result;

    /* data setup */
    rectA.x=RandomIntegerInRange(-1024, 1024);
    rectA.y=RandomIntegerInRange(-1024, 1024);
    rectA.w=RandomIntegerInRange(1, 1024);
    rectA.h=RandomIntegerInRange(1, 1024);
    rectB.x=RandomIntegerInRange(-1024, 1024);
    rectB.y=RandomIntegerInRange(-1024, 1024);
    rectB.w=RandomIntegerInRange(1, 1024);
    rectB.h=RandomIntegerInRange(1, 1024);

    // invalid parameter combinations
    result = SDL_RectEquals((const SDL_Rect *)NULL, (const SDL_Rect *)&rectB);
    AssertTrue(result == SDL_FALSE, "Function did not return FALSE when 1st parameter was NULL"); 
    result = SDL_RectEquals((const SDL_Rect *)&rectA, (const SDL_Rect *)NULL);
    AssertTrue(result == SDL_FALSE, "Function did not return FALSE when 2nd parameter was NULL"); 
    result = SDL_RectEquals((const SDL_Rect *)NULL, (const SDL_Rect *)NULL);
    AssertTrue(result == SDL_FALSE, "Function did not return FALSE when 1st and 2nd parameter were NULL"); 
}
