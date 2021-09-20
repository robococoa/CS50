SELECT DISTINCT name FROM people
    JOIN stars ON people.id = stars.person_id
    JOIN movies ON stars.movie_id = movies.id
    WHERE year = "2004"
    AND birth > "1"
    ORDER BY birth;