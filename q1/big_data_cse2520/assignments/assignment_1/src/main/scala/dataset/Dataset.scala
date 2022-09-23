package dataset

import dataset.util.Commit.Commit
import java.text.SimpleDateFormat

/**
 * Use your knowledge of functional programming to complete the following functions.
 * You are recommended to use library functions when possible.
 *
 * The data is provided as a list of `Commit`s. This case class can be found in util/Commit.scala.
 * When asked for dates, use the `commit.commit.committer.date` field.
 *
 * This part is worth 40 points.
 */
object Dataset {


  /** Q23 (4p)
   * For the commits that are accompanied with stats data, compute the average of their additions.
   * You can assume a positive amount of usable commits is present in the data.
   *
   * @param input the list of commits to process.
   * @return the average amount of additions in the commits that have stats data.
   */
  def get_additions(input: List[Commit]): List[Int] = input match {
    case Nil => Nil
    case head :: tail if (head.stats.isDefined) => head.stats.get.additions :: get_additions(tail)
  }

  def avgAdditions(input: List[Commit]): Int = {
    get_additions(input).sum / get_additions(input).length
  }

  /** Q24 (4p)
   * Find the hour of day (in 24h notation, UTC time) during which the most javascript (.js) files are changed in commits.
   * The hour 00:00-00:59 is hour 0, 14:00-14:59 is hour 14, etc.
   * NB!filename of a file is always defined.
   * Hint: for the time, use `SimpleDateFormat` and `SimpleTimeZone`.
   *
   * @param input list of commits to process.
   * @return the hour and the amount of files changed during this hour.
   */
  def getFileType(filename: Option[String]): String = filename match {
    case Some(x) => x.split('.').last
    case None => ""
  }

  def jsTime(input: List[Commit]): (Int, Int) = {
    val jsCommitsCount = input.map(commit => commit.files.count(file => getFileType(file.filename) == "js"))
    val dates = input.map(commit => commit.commit.committer.date)
    val hours = dates.map(date => new SimpleDateFormat("HH").format(date))

    // Correct for timezone
    val timezones = dates.map(date => new SimpleDateFormat("Z").format(date))
    val hoursCorrected = hours.zip(timezones).map(x => x._1.toInt - x._2.toInt / 100)

    // Zip and order
    val jsCommitsPerHour = jsCommitsCount.zip(hoursCorrected).groupBy(_._2).mapValues(_.map(_._1).sum)
    val max = jsCommitsPerHour.maxBy(_._2)
    max
  }

  /** Q25 (5p)
   * For a given repository, output the name and amount of commits for the person
   * with the most commits to this repository.
   * For the name, use `commit.commit.author.name`.
   *
   * @param input the list of commits to process.
   * @param repo  the repository name to consider.
   * @return the name and amount of commits for the top committer.
   */
  def topCommitter(input: List[Commit], repo: String): (String, Int) = {
    val repos = input.map(commit => commit.url.split("/").slice(4, 6).mkString("/"))
    val names = input.map(commit => commit.commit.author.name)
    val filtered_repos_names = repos.zip(names).filter(_._1 == repo)
    val grouped = filtered_repos_names.groupBy(_._2).mapValues(_.size)
    val max = grouped.maxBy(_._2)
    max
  }

  /** Q26 (9p)
   * For each repository, output the name and the amount of commits that were made to this repository in 2019 only.
   * Leave out all repositories that had no activity this year.
   *
   * @param input the list of commits to process.
   * @return a map that maps the repo name to the amount of commits.
   *
   *         Example output:
   *         Map("KosDP1987/students" -> 1, "giahh263/HQWord" -> 2)
   */
  def commitsPerRepo(input: List[Commit]): Map[String, Int] = {
    val repos = input.map(commit => commit.url.split("/").slice(4, 6).mkString("/"))
    val dates = input.map(commit => commit.commit.committer.date)

    // TODO: Get better year extraction
    val filtered_repo_dates = repos.zip(dates).filter(_._2.getYear == 119)
    val grouped = filtered_repo_dates.groupBy(_._1).mapValues(_.size)
    grouped
  }


  /** Q27 (9p)
   * Derive the 5 file types that appear most frequent in the commit logs.
   * NB!filename of a file is always defined.
   * @param input the list of commits to process.
   * @return 5 tuples containing the file extension and frequency of the most frequently appeared file types, ordered descendingly.
   */
  def topFileFormats(input: List[Commit]): List[(String, Int)] = {
    val filetypes = input.map(commit => commit.files.map(file => getFileType(file.filename)))
    val flattened = filetypes.flatten
    val grouped = flattened.groupBy(identity).mapValues(_.size)
    val sorted = grouped.toList.sortBy(_._2).reverse
    sorted.take(5)
  }


  /** Q28 (9p)
   *
   * A day has different parts:
   * Morning 5 am to 12 pm (noon)
   * Afternoon 12 pm to 5 pm.
   * Evening 5 pm to 9 pm.
   * Night 9 pm to 4 am.
   *
   * Which part of the day was the most productive in terms of commits ?
   * Return a tuple with the part of the day and the number of commits
   *
   * Hint: for the time, use `SimpleDateFormat` and `SimpleTimeZone`.
   */
  def hoursToParts(hour: Int): String = hour match {
    case x if (x >= 5 && x < 12) => "morning"
    case x if (x >= 12 && x < 17) => "afternoon"
    case x if (x >= 17 && x < 21) => "evening"
    case _ => "night"
  }

  def mostProductivePart(input: List[Commit]): (String, Int) = {
    val dates = input.map(commit => commit.commit.committer.date)
    val hours = dates.map(date => new SimpleDateFormat("HH").format(date))

    // Correct for timezone
    val timezones = dates.map(date => new SimpleDateFormat("Z").format(date))
    val hoursCorrected = hours.zip(timezones).map(x => x._1.toInt - x._2.toInt / 100)

    // Zip and order
    val parts = hoursCorrected.map(hoursToParts)
    val grouped = parts.groupBy(identity).mapValues(_.size)
    val max = grouped.maxBy(_._2)
    max
  }
}
