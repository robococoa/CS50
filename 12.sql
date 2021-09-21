SELECT title FROM movies
    JOIN stars ON movies.id = stars.movie_id
    JOIN people ON stars.person_id = people.id
    WHERE movie_id IN (SELECT movie_id from stars WHERE name = "Johnny Depp" AND name = "Helena Bonham Carter");