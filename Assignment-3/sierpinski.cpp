/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    /* TODO: Implement this function. */
    if (order != 0) {
        drawSierpinskiTriangle(window, {one.x, one.y}, {(one.x + two.x) / 2, (one.y + two.y) / 2}, {(one.x + three.x) / 2, (one.y + three.y) / 2}, order - 1);
        drawSierpinskiTriangle(window, {(one.x + two.x) / 2, (one.y + two.y) / 2}, {two.x, two.y}, {(two.x + three.x) / 2, (two.y + three.y) / 2}, order - 1);
        drawSierpinskiTriangle(window, {(one.x + three.x) / 2, (one.y + three.y) / 2}, {(two.x + three.x) / 2, (two.y + three.y) / 2}, {three.x, three.y}, order - 1);
    }else {
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    return 0;
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

