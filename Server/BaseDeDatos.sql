CREATE DATABASE IF NOT EXISTS `tres_en_raya_online`;
USE `tres_en_raya_online`;

CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nickname VARCHAR(30),
    password_hash VARCHAR(255)
);

CREATE TABLE IF NOT EXISTS rankings (
    user_id INT PRIMARY KEY,
    nickname VARCHAR(30),
    points INT,
    wins INT,
    losses INT,
    draws INT
);

CREATE TABLE IF NOT EXISTS matches (
    id INT AUTO_INCREMENT PRIMARY KEY,
    room_code VARCHAR(20),
    status ENUM('waiting', 'in_progress', 'finished', 'cancelled'),
    created_at DATETIME,
    finished_at DATETIME
);

CREATE TABLE IF NOT EXISTS match_reports (
    id INT AUTO_INCREMENT PRIMARY KEY,
    match_id INT,
    reporting_user_id INT,
    reported_order VARCHAR(255),
    created_at DATETIME
);