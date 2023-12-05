Keoni Burns  
CSCI 551

# Exercise 4

## Q1

### Code implementation

the code was adapted from the interpolation.c provided in the starter code

The logic behind is as follows:

- define step size to be used for incrementation between discrete time values
- resize the number of values given the step size
  - ie when $steps=10, newSize = (10 \times 1800) = 18,000$
- using our new size we can iterate the appropriate amount of times
  - each iteration we multiply the iterator by $\Delta t$ and pass this in to our faccel function
- faccel
  - this function uses type casting to manipulate the acutal time value into discrete values
  - it finds our current discrete value and our next discrete value and then creates our $\Delta t$ variable using the time we passed in minus the discrete time value
  - we then return the difference between our next and current time acceleration values multiplied by our change in time
- table accel
  - this literally just returns the acceleration value from the default profile values
  - also checks that the time given is appropriate

### output

![my_csv](./ss/ex1csv.jpeg)

![given_csv](./ss/givenAccel.jpg)

as you can see from the two outputs there is a bit of discrepency between my output and the given non-interpolated values. I believe the reason for this can be attributed to a couple of different factors.

1. the input for the two outputs may have been different which is human error on my part
2. the nature of interpolation: being that the actual method for interpolating between values is multiplying the difference between to acceleration values we can asssume that the difference between outputs is due to this. however in the implementation we are not summing the interpolated values at whole numbers.

in conclusion, i found the reason for the difference is because i used the ex6lin.h for a lookup table instead of the one from the provided xlsx file. However, should we sum each of the values up until the next whole time value i can confidently say that the acceleration values at each would be different.

![my_chart](./ss/ex1.png)

![given_chart](./ss/givenaccelchart.jpg)

here is my graph for the ex6lin.h acceleration values for acceleration in comparison to the given acceleration graph

## Q2)

### code implementation

for this program I built upon the mpi_trap4.c program provided. The only real change I made was the implementation of Simpsoons integration rule.

the logic for the program is as follows:

- we first initialize our nodes and grab user input which is done through the Get_input function
  - this function prompts the user to define start and endpoints as well as number of slices from rank 0, the controller
  - additionally it broadcasts the data from the user to each of the nodes
- after which we create $\Delta x$ by taking the difference between the start and end and dividing it by the number of alotted slices using the following: $\boxed {h = {(b-a) \over n} } $
  - we also find the size of each quadrature by dividing the number of slices by the number of nodes working on the problem
- we then find our local start and endpoints using the step size and the local quadrature size

  - for rank 0 with 4 nodes and 1000 rectangles :

  $\boxed{local_b = local_a + (local_n \times h) = 0 + (250\times1.8) = 450 }$

- from this point we can pass our constraints into each method of integration
- leftRiemann
  - the function takes in our constraints and iterates from 1 to our endpoint
    - on each iteration the function finds the area by multiplying the left x value by the output from our function that returns the sin value of the given input
    - we then add the product to the area and incremement by $\Delta x$ and rinse and repeat
- Trap
  - we start by creating our initial estimate then we iterate from 1 to the number of slices
    - each iteration we increment x using the product of i and $\Delta x$ and pass our new x into our sin function and add the value to our estimate
    - we also multiply our final sum by $\Delta x$
- Simpsons

$\boxed {Integral \approx {h \over 3} [f(a) + f(b) + (4 \times sum(even\ midpoints)) + (2 \times sum(odd\ midpoints))]}$

Where:

- h is the width of each subinterval, which is (b - a) / n.
- f(a) and f(b) are the function values at the endpoints.
- "sum(even midpoints)" represents the sum of 4 times the function values at midpoints of even-indexed subintervals.
- "sum(odd midpoints)" represents the sum of 2 times the function values at midpoints of odd-indexed subintervals.

![simpsons](./ss/simpsons.jpg)

### output

![my_output](./ss/ex2out.jpg)

$$\boxed{{(1.991221027222030-2.0) \over 2.0}  \times 100 \approx\ 0.4389 \% }$$

- as seen we still have a relatively high margin of error however i couldn't add anymore slices with out printing our garbage values and this implementation is also not as optimized as it could be

from the given output we can see that the simpsons implementation was the closest to 2.0 however the difference between it's output and the trapezoidal implement is: $\boxed {1.991221027222030-1.991221027213916 \approx 0.000000000008114}$ which is almost negligble given that its in the 13th digits place

however, in precision critical circumstances then I would use simpsons. The reason being that Simpson's uses second-order polynomials over each subinterval and so the approximation is closer to the true integral of the function. Additionally, since we are using a smooth curve function, sin, simpsons is going to be the most accurate. Trapezoidal integration is a close second however because it uses straight lines from each of the subintervals it introduces a small margin of error. Left Riemann is more suited for discontinuous functions or piece-wise functions because it doesn't need to consider intermediary points of the function allowing it to approximate more accurately than either of the other implementations

### run-time

![vanilla](./ss/vanillaex2.jpg)

![my_runtime](./ss/ex2time.jpg)

$$\boxed{speed \ up = {23.723s \over 12.499s} \approx 1.89872}$$

- our program ran 1.8 times faster using mpi

$$\boxed{P = { N \times {1-(1 \div S) \over  (N-1)}} \ }$$
$$\boxed{P \approx 0.5412}$$

- using the above formula we see that only 54% of the program was parallelized and could be further improved upon however with scaling you would see a larger increase in speed-up

## Q3)

### code implementation

for this program I was unable to get the interpolation to work for both velocity and position in one program because I could not figure out how to adapt the compare code to use interpolation. Had I started a bit earlier I would have gotten it to work however we can't cry over spilt milk can we.

the logic for the program is the same as the previous exercise however instead of using sin we use table interpolation which is about the same as the logic from exercise 1 where we take the difference between our two discrete values and multiply them by our dt.

### outputs

![ex3a](./ss/ex3a.jpg)

the above image is using $\Delta t = 0.001$

![ex3b](./ss/ex3breal.jpg)

the above image is using $\Delta t = 0.0001$

![ex3bextra](./ss/ex3b.jpg)

the above image is using $\Delta t = 0.00001$

something interesting is that with more slices or subintervals per second we can see that the velocity at t = 1800 slowly goes from negative to positive. This may be due to the approximation becoming more accurate with smaller steps allowing for the decelleration to become positive. one possibility is that because theres more slices to sum up the negative numbers become smaller and so with that came along a more accurate depiction of what acutally happens at t = 1800. Otherwise it would be that our train is moving backwards at t = 1800 which is entirely possible at the scale of the velocity but seems unlikely unless due to the jerking of decelleration

### runtime and scalability

![ex3cacc](./ss/ex3vel.jpg)

the output above is the interpolation of acceleration at 2, 4, 8, 16 processses

![ex3cvel](./ss/ex3acc.jpg)

the output above is the interpolation of velocity at 2, 4, 8, 16 processses

![chart](./ss/procgraph.jpg)

analysis: as we can see the number of slices isnt scaled up enough to see considerable change in the times between runs. Additionally, there appears to be a dip around the range of 4-8 processes and the only thing i can chalk that up to is overhead for mpi

![comp](./ss/comparison.jpg)

$$\boxed{speed \ up = {11.096s \over 4.205s} \approx 2.641}$$

- our program ran 2.6 times faster using mpi with 16 cores

  $$\boxed{P = { N \times {1-(1 \div S) \over  (N-1)}} \ }$$
  $$\boxed{P \approx 0.6633}$$

  - from thiss we can see that 66% of our program is parallelized

  conclusion: given the circumstance of the problem we can see that it isn't scaled enough to see significant speed however we do know that it iss possible. Additionally it would appear that both our accuracy and precision scale up with smaller step sizes.
