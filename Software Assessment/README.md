Part 1

The methods and data structures used are explained in the .hpp files
in Software Assessment 1 folder. Overall, I used C++ and the built in GLUT library to do the rendering.

The Algorithm I used to compute the highlighted GridPoints is also documented, but goes as follows:

    Loop over each GridPoint and look to its right neighbor if it has one. If the signed distances (to the circle's edge)  of each neighbor is opposing signs, the circle boundary crosses between the two neighbors. Whichever neighbor has the smallest signed distance in magnitude gets highlighted.

    For each highlighted GridPoint, compute the closest and furthest corner distance to the center of the blue circle. If the furthest distance is larger than outerRadius, update outerRadius. If the closest distance is smaller than innerRadius, update innerRadius.

    Repeat this process, but for vertical neighbors.

    Draw Red Circles based on innerRadius and outerRadius.

To compile and run part 1, go to the root directory of Software Assessment 1 and run 

'g++ -framework OpenGL -framework GLUT -o main *.cpp'

and then run

'./main'

NOTE: I used a Mac to develop this which is why I was unable to produce an executable for Windows. I briefly researched and believe all of my code as well as these commands should work on Windows, but I apologize in advance if it is troublesome.

A g++ compiler is needed.



Part 2

The methods and data structures I used are well documented in the .hpp files, but I will summarize below my Least-Squares Algorithm.

To start I needed Matrix operation abilities, so in MatrixUtils I have datastructures to support Matrix multiplication, transposition, inversion (using Gauss-Jordan Elimination) as well as a few others.

Second, in order to do least squares, I needed an algorithm, and used 
the Gauss-Newton algorithm as prescribed here:

https://en.wikipedia.org/wiki/Gauss%E2%80%93Newton_algorithm

This requires computing (J_T * J)^-1 * J_T, where J is the Jacobian and J_T is its transpose. Finding the Jacobian required finding the partial derivatives of the residuals with respect to each unknown parameter.

The residuals were distance based, so for a circle, just the distance of an input point to the circle's current center, minus the current radius of the circle. For the ellipse it was more complex.

I did 100 iterations of the Gauss Newton algorithm to compute the final parameters for both circles and ellipses.

The parameters for the circles were just its center and radius, so 
[x_c, y_c, r], so as long as there are >=3 highlighted points, the Jacobian will be nonsingular and a circle will generate. Sometimes it will still work if you only highlight 2 points. Further, if the all points are colinear, the circle generator might not work.

The parameters for the ellipse were
[x_c, y_c, a, b, alpha]
where (x_c, y_c) is the ellipse center, a and b are the semi-major/minor axes, respectively, and alpha is a rotation parameter measuring the offset of the semi-major axis from the positive x-axis.

This leaves a Jacobian which is m x (m+5), since computing the distance of a point to an ellipse requires knowing an angle phi on the ellipse closest to the sample point. So for m given points, there are m phi's which also must be computed as an unknown parameter. This leaves us with a singular Jacobian every time though. The solution I used was to break the residuals down into x and y components, and minimizing them separately. This gives us 2m equations with m+5 unknowns. Therefore, as long as you input >=5 highlighted points, the algorithm will be able to compute to completion. However, there are many degrees of freedom here including the possibility for a < 0 or b < 0, which I tried to actively protect against. This freedom makes the ellipse generator a little buggy, so sometimes an ellipse will not generate or will generate an ellipse that doesn't fit great, but most of the time I would say it fits pretty well.

To compile and run, use the same instructions above for part 1, but in the Software Assessment 2 directory.
