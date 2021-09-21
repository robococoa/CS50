SELECT name FROM people
WHERE id IN
    (SELECT person_id FROM stars WHERE movie_id IN
    (SELECT movie_id FROM movies
        JOIN stars ON movies.id = stars.movie_id
        JOIN people ON stars.person_id = people.id
        WHERE person_id = (SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = "1958")))
AND name IS NOT "Kevin Bacon";