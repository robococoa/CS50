-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Check crime scene reports for details of the duck robbery, (simplified to the relevant id from the search results) id = 295: 10:15am at the Chamberlin Street courthouse, three witnesses, mentions the courthouse
SELECT DESCRIPTION FROM crime_scene_reports WHERE id = '295';

-- Check interviews from July 28th, 2020 from problem description, and "courthouse" mentioned from the crime scene report
SELECT id, name, year, month, day, transcript FROM interviews WHERE day = 28 AND month = 7 AND year = "2020" AND transcript LIKE "%courthouse%";

-- Ruth saw the thief get into a car within ten mins of 10:30am; Eugene recognised the face from the person at the ATM on Fifer Street before 10:30am; Raymond saw the thief make a < 1 min call for the accomplice to purchase the earliest flight out of Fiftyville tomorrow (29/7/2020)
-- Security footage for the Car license plate for 28/7/2020 during 10th hour, with exit activity between 10:30am and 10:40am
 SELECT id, month, day, hour, minute, activity, license_plate FROM courthouse_security_logs WHERE day = 28 AND month = 7 AND year = '2020' AND hour = 10 AND activity = 'exit' AND minute BETWEEN 30 AND 40;
-- Resulting licence plate is 1106N58

-- Find out airport for Fiftyville
SELECT id, abbreviation, full_name, city FROM airports;
-- Fiftyville Regional Airport code is CSF = id 8

-- Check earliest flight out of CSF:8, on 29/7/2020
SELECT id, origin_airport_id, destination_airport_id, year, month, day, hour, minute FROM flights WHERE year = '2020' AND month = 7 AND day = 29 AND origin_airport_id = 8;
-- Result is 8:20am to ID 4

-- Check City for ID 4
SELECT id, abbreviation, full_name, city FROM airports WHERE id = 4;
-- Result is LHR, Heathrow Airport, London

-- Check owner of license plate 1106N58
SELECT id, name, phone_number, passport_number, license_plate FROM people WHERE license_plate = '1106N58';
-- Result is 449774 | Madison | (286) 555-6063 | 1988161715 | 1106N58

-- Check phone records for 28/7/2020 for a < 1 min call where caller is (286) 555-6063
SELECT id, caller, receiver, year, month, day, duration FROM phone_calls WHERE year = '2020' AND day = 28 AND month = 7 AND duration < 60 AND caller = '(286) 555-6063';
-- Result is 254 | (286) 555-6063 | (676) 555-6554 | 2020 | 7 | 28 | 43

-- Check owner of phone number (676) 555-6554
SELECT id, name, phone_number, passport_number, license_plate FROM people WHERE phone_number = '(676) 555-6554';
-- Result is 250277 | James | (676) 555-6554 | 2438825627 | Q13SVG6
-- James is the accompalice

-- Check passengers with passports for Madison
SELECT flight_id, passport_number, seat FROM passengers WHERE passport_number = '1988161715';
-- Result has flight 36, seat 6D

-- Check bank accounts for information for James: ID 250277
SELECT account_number, person_id, creation_year FROM bank_accounts WHERE person_id = '250277';

-- Check bank accounts for information for Madison: ID 449774
SELECT account_number, person_id, creation_year FROM bank_accounts WHERE person_id = '449774';
-- Result 76054385 | 449774 | 2015
-- Confirm this account was used on the ATM record before 10:30am on 28/7/2020 on Fifer Street, with Madison's account number
SELECT id, account_number, year, month, day, atm_location, amount FROM atm_transactions WHERE month = 7 AND day = 28 AND atm_location = "Fifer Street" AND account_number = '76054385';
-- Result 266 | 76054385 | 2020 | 7 | 28 | Fifer Street | 60
-- Madison is the thief

-- Check for passengers on 8:20am flight to London
SELECT flight_id, passport_number, seat FROM passengers;