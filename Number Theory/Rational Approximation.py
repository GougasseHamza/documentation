'''
Problem it Solves: Rational Approximation using Python's Fraction module.
Usage: Fraction(x).limit_denominator(limit).
Input/Output: x -> Fraction.
Time Complexity: Efficient.
Tweaks: Limit denominator.
Other Use Cases: Precision math.
Warnings: Python specific.
'''

from fractions import Fraction

def main():
    x = 3232.45622121
    # gets the closest rational number to x where denominator is limited
    p = Fraction(x).limit_denominator(2323)
    print(p)

if __name__ == '__main__':
    main()
