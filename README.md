FOLLOW THESE SQL CODE FOR DATA INSERTION

create database trainapp;
use trainapp;

create table appaccount(
id int primary key auto_increment,
name varchar(225),
phone varchar(225) unique,
aadhar varchar(225) unique,
address varchar(300) not null,
dob varchar(225),
gmail varchar(225) unique,
bank_number varchar(225) not null,
password varchar(225) not null);

create table reservation(
id int primary key auto_increment,
passenger_name varchar(225),
age int,
 phone varchar(225) ,
 bill double,
 date varchar(225),
 village varchar(225) );

 create table ticket_history(
history varchar(225));
