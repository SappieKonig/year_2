package fp_practice

import scala.annotation.tailrec

/**
  * In this part you can practice your FP skills with some small exercises.
  * Hint: you can find many useful functions in the documentation of the List class.
  *
  * This part is worth 15 points.
  */
object FPPractice {

    /** Q20 (4p)
      * Returns the sum of the first 10 numbers larger than 25 in the given list.
      * Note that 10 is an upper bound, there might be less elements that are larger than 25.
      *
      * @param xs the list to process.
      * @return the sum of the first 10 numbers larger than 25.
      */
    def first10Above25(xs: List[Int], index: Int = 0): Int = xs match {
        case Nil => 0
        case x :: xs => if (x > 25 && index < 10) x + first10Above25(xs, index + 1) else first10Above25(xs, index)
    }

    /** Q21 (5p)
      * Provided with a list of all grades for each student of a course,
      * count the amount of passing students.
      * A student passes the course when the average of the grades is at least 5.75 and no grade is lower than 4.
      *
      * @param grades a list containing a list of grades for each student.
      * @return the amount of students with passing grades.
      */
    def passingStudents(grades: List[List[Int]]): Int = grades match {
        case Nil => 0
        case x :: xs => if (x.sum / x.length >= 5.75 && x.min >= 4) 1 + passingStudents(xs) else passingStudents(xs)
    }

    /** Q22 (6p)
      * Return the length of the first list of which the first item's value is equal to the sum of all other items.
 *
      * @param xs the list to process
      * @return the length of the first list of which the first item's value is equal to the sum of all other items,
      *         or None if no such list exists.
      *
      * Read the documentation on the `Option` class to find out what you should return.
      * Hint: it is very similar to the `OptionalInt` you saw earlier.
     */
    @tailrec
def headSumsTail(xs: List[List[Int]]): Option[Int] = xs match {
        case Nil => None
        case x :: xs => if (x.head == x.tail.sum) Some(x.length) else headSumsTail(xs)
    }
}
