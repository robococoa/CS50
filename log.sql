-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Check crime scene reports for details of the duck robbery, (simplified to the relevant id from the search results) id = 295: 10:15am at the Chamberlin Street courthouse, three witnesses, mentions the courthouse
SELECT DESCRIPTION FROM crime_scene_reports WHERE DESCRIPTION LIKE "%Chamberlin Street%";

-- Check interviews from July 28th, 2020 from problem description, and "courthouse" mentioned from the crime scene report
SELECT id, name, year, month, day, transcript FROM interviews WHERE day = 28 AND month = 7 AND year = "2020" AND transcript LIKE "%courthouse%";

-- Ruth saw the thief get into a car within ten mins of 10:15am; Eugene recognised the face from the person at the ATM on Fifer Street before 10:15am making a withdrawal; Raymond saw the thief make a < 1 min call for the accomplice to purchase the earliest flight out of Fiftyville tomorrow (29/7/2020)
-- Security footage for the Car license plate for 28/7/2020 during 10th hour, with exit activity between 10:15am and 10:25am
 SELECT id, month, day, hour, minute, activity, license_plate FROM courthouse_security_logs WHERE day = 28 AND month = 7 AND year = '2020' AND hour = 10 AND activity = 'exit' AND minute BETWEEN 15 AND 25;
-- Resulting licence plates IDs are 260-267

-- Possible thief accounts from before 10:15am, at Fifer Street ATM
SELECT id, account_number, transaction_type FROM atm_transactions WHERE year = '2020' AND month = 7 AND day = 28 AND transaction_type = 'withdraw';
-- Multiple results

-- Find out airport for Fiftyville
SELECT id, abbreviation, full_name, city FROM airports;
-- Fiftyville Regional Airport code is CSF = id 8

-- Check earliest flight out of CSF:8, on 29/7/2020
SELECT id, origin_airport_id, destination_airport_id, year, month, day, hour, minute FROM flights WHERE year = '2020' AND month = 7 AND day = 29 AND origin_airport_id = 8;
-- Result is 8:20am to ID 36 to destination 4

-- Possible passports for passengers on 8:20am flight to London, flight ID 4
SELECT passport_number FROM passengers WHERE flight_id = 36;
-- Multiple results

-- Check City for ID 4
SELECT id, abbreviation, full_name, city FROM airports WHERE id = 4;
-- Result is LHR, Heathrow Airport, London
-- This is the thief's destination

-- Phone records for 28/7/2020 for a < 1 min call
SELECT id, caller, receiver, year, month, day, duration FROM phone_calls WHERE year = '2020' AND day = 28 AND month = 7 AND duration < 60;
-- 9 results

-- Possible thieves with matching license plates in question and phone numbers from callers in question, and passports on flight 36
SELECT name FROM people
WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = '2020' AND day = 28 AND month = 7 AND duration < 60)
AND license_plate IN (SELECT license_plate FROM courthouse_security_logs WHERE day = 28 AND month = 7 AND year = '2020' AND hour = 10 AND activity = 'exit' AND minute BETWEEN 15 AND 25)
AND passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = 36)
-- Roger, Evelyn, Ernest
AND id IN (
SELECT person_id FROM bank_accounts
WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = '2020' AND month = 7 AND day = 28 AND transaction_type = 'withdraw'));
-- Ernest is the only result
-- Ernest is the thief


-- Accomplice is receiver from thief's phone number on that day
SELECT name FROM people WHERE phone_number = (
SELECT receiver FROM phone_calls WHERE year = '2020' AND day = 28 AND month = 7 AND duration < 60
AND caller = (SELECT phone_number FROM people WHERE name = 'Ernest'));
-- Berthold is the only result
-- Berthold is the accomplice