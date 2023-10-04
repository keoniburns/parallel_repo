Keoni Burns  
report 3  
CSCI 551

# Exercise 3

## Q1) Left Riemann Sum

### Code

---

![macros](./ss/macros.jpg)

![main](./ss/Leftriemann_main.jpg)

![left_riemann](./ss/left_func.jpg)

above are my changes to the code for both left and trapezoidal riemann sum programs

- created macros for the functions that calaculate the scaling factors for our given circumstance
- switched from c to cpp because i felt more comfortable using the lang
- added a time struct to keep track of monotonic raw clock time
- additionally i added trap function to the left_riemann.cpp because they both use floats and so it sseemed appropriate

### Speed Up

---

single threaded

![single_left](./ss/single_left.jpg)

mpi implementation

![mpi_left](./ss/left_area.jpeg)

$$\boxed{speed \ up = {0.002259s \over 0.003557s} \approx 0.63} $$

- there is minor speed up but not significant
  - this is due to the problem scaling and our questions not explicitly pertaining to speed up but rather accuracy

### Accuracy and Precision

---

the formula for percent error is as follows:  
$$\boxed{(measured \ value - theoretical) \over true \ value} $$

- from the image above we can see that our value at t = 1800 is 122449.8906250

substuting values from the given excel sheet as our theoretical we get

$$\boxed{{(122449.8906250-122000) \over 122000}  \times 100 \approx0.36829 \ percent \ error}$$

- as we can see there is a 0.36% margin of error with this result for position approximation

![left_accel](./ss/left_velocity.jpg)

unfortunately the velocity my program produced was not correct and should we try to plug it into the formula we would get infinity because we would be dividing by 0

### Analysis

---

- My results for position have a 0.36% error when tested against the theoretical value given to us

  - this error percent is most likely due to floating point being unable to hold that much data given that it is only 4 bytes, 32 bits. Ergo precision can only be guaranteed up to 7 decimal places. This should not matter though because we were off by about 450 meters (this would not be good in real life)

  - the results from acceleration are most likely due to inefficient programming on my part. Were we to get accurate results we should have outputted something along the lines of 3x10^-14.
    - it should be noted that running on a single thread my acceleration results were accurate and more correct. i'm not too sure what happened along the way.

## Q2) Mid-Riemann sum

### Code

---

![mid_riemann](./ss/mid_func.jpg)

the image shows my implementation of mid-point riemann

- it should be noted that program is nearly identical to my left_riemann.cpp and the only differences are this function and the typecasting of the variables from float to double

### Speed Up

---

single threaded

![single_mid](./ss/single_mid.jpg)

using mpi

![mpi_mid](./ss/mid_area.jpg)

$$\boxed{speed \ up = {0.013723s \over 0.028261s} \approx 0.485}$$

- once again we can see that our speed up isn't by any means impressive and it is due to the scale of our problem and what our objectives are in this exercise.

### Accuracy and Precision

---

if we use our percent error formula on the area we measured against the theoretical we get the following:

$$\boxed{{(122406.65366551974148-122000) \over 122000}  \times 100 \approx0.33308 \ percent \ error}$$

- it can be seen that once again our percent error, eventhough below a single percent, is still significant enough to be dangerous in critical situations. 400 meters is a large distance, its about 3.5 football fields.

![mid_vel](./ss/mid_velocity.jpg =250x)

- once again the velocity at t=1800 is not nearly small enough to be considered correct and were it to be an accurate value it would be smaller by about 10~12 orders of magnitude

### Analysis

---

sources of error

- poor implementation of code
- accuracy was simply not there which may have been due to how i was dividing up the work between the processes

notes

- precision didn't seem to be nearly as much of an error simply because the data i got was not accurate enough to give a precision error.

## Q3 & Q4) Trapezoidal Riemann Sum

### Code

---

![trap](./ss/left_func.jpg)

- the image above showss the logic of my trap program which took inspiration from some starter code given

### Speed Up

---

single threaded

![single_trap](./ss/single_trap.jpg =x250)

mpi trap

![mpi_trap](./ss/double_trap_area.jpg)

$$\boxed{speed \ up = {0.025884s \over 0.040999s} \approx 0.631}$$

- not significant enough to make any remarks about but it seemed relevant to include

### Accuracy and Precision

---

![trap_pos_double](./ss/double_trap_area.jpg)

$$\boxed{{(122387.46994922136946-122000) \over 122000}  \times 100 \approx 0.3192\ percent \ error}$$

![trap_pos_float](./ss/float_trap_area.jpg)

$$\boxed{{(122410.250000000-122000) \over 122000}  \times 100 \approx 0.3368\ percent \ error}$$

- from the two trap results for position at t=1800, we can see that neither are very accurate(a common theme throughout this report). This is due to my implementation. Both are below 1 percent however neither are nearly accurate enough to be deemed practically correct.
- interestingly we see the lack of precision on the float typecasted implementation maxing out at 0.25

![trap_vel_double](./ss/double_trap_velocity.jpg)

![trap_vel_float](./ss/float_trap_velocity.jpg)

- these results are not accurate enough to be reliable.

### Analysis

---

sources of error:

- I believe the source of error for these are mainly due to implementation and i will go over what i would do to improve these results and my thoughts on how my implementation is flawed
- typecasting for float didn't hold and had the approximation been accurate there definitely would have been precision errors.

## Summary

---

- faulty code
  - I do believe that these could have been better had i managed my time more effectively
  - one possible downfall in my code may have been that i was double counting at the edges of each section and so i may have been double counting values and adding them to the final result.
    - i would have to look more into it but i believe this is the reason for my positions having such a large margin of error
  - another possible error is that I was using the same implementationss and switching which antiderivative being used for pos i used velocity(x) and for velocity i used acceleration
    - there may have been a human error where i mightve had a combination of the two insted of one or the other because i forgot to comment out and uncomment the respective function calls.
  - finally it may have been because i was unknowingly using cmath library instead of math.h, however, im unsure how much this would have changed the results from the program.
