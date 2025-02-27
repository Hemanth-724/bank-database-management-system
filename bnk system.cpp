drop database eee2;

create database eee2;

use eee2;
-- 2. Tables for Customers, Accounts, and Transactions

CREATE TABLE Customers (
  account_number INT PRIMARY KEY AUTO_INCREMENT,   
  name VARCHAR(50) NOT NULL,
  address VARCHAR(100),
  phone_number CHAR(10)
);

select * from customers;

CREATE TABLE Accounts (
  account_number INT PRIMARY KEY,
  account_type ENUM('Savings', 'Current') NOT NULL,   -- An ENUM data type restricts a column to a predefined set of possible values
  balance DECIMAL(10,2) NOT NULL DEFAULT 0.00,
  FOREIGN KEY (account_number) REFERENCES Customers(account_number)
);

CREATE TABLE Transactions (
  transaction_id INT PRIMARY KEY AUTO_INCREMENT,
  account_number INT NOT NULL,
  transaction_type ENUM('Credit', 'Debit') NOT NULL,
  amount DECIMAL(10,2) NOT NULL,
  transaction_date DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (account_number) REFERENCES Accounts(account_number)
);

-- 3. Stored Procedures
CALL CreateCustomerAccount("p","ciH",09090907);

-- a) Create Customer Account
DELIMITER //
CREATE PROCEDURE CreateCustomerAccount (
  IN customer_name VARCHAR(50),
  IN customer_address VARCHAR(100),
  IN customer_phone_number CHAR(10)
)
BEGIN
  DECLARE new_account_number INT;
  
  INSERT INTO Customers (name, address, phone_number) VALUES (customer_name, customer_address, customer_phone_number);
  SET new_account_number = LAST_INSERT_ID();
  
  INSERT INTO Accounts (account_number, account_type) VALUES (new_account_number, 'Current');  -- Assuming default account type is Current
  
  SELECT new_account_number AS account_number;
END //
DELIMITER ;

-- b) Credit Account
DELIMITER //
CREATE PROCEDURE CreditAccount (
  IN p_account_number INT,
  IN p_amount DECIMAL(10,2)
)
BEGIN
DECLARE v_balance DECIMAL(10,2); -- this stores the current balance before credit of amount
  
 
  -- Check if the account exists
  SELECT balance INTO v_balance FROM Accounts WHERE account_number = p_account_number LIMIT 1;
  
  IF FOUND_ROWS() = 0 THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Account not found';
  ELSE
    -- Update the balance
    UPDATE Accounts SET balance = balance + p_amount WHERE account_number = p_account_number;
    
    -- Insert transaction record
    INSERT INTO Transactions (account_number, transaction_type, amount) VALUES (p_account_number, 'Credit', p_amount);
  END IF;
END //
DELIMITER ;

-- Add unique constraint to account_number in Customers table
ALTER TABLE Customers ADD CONSTRAINT UNIQUE (account_number);

-- Modify DebitAccount procedure
DROP PROCEDURE IF EXISTS DebitAccount;
DELIMITER //
CREATE PROCEDURE DebitAccount (
  IN p_account_number INT,
  IN p_amount DECIMAL(10,2)
)
BEGIN
  DECLARE current_balance DECIMAL(10,2);
  
  -- Lock the row for update
  SELECT balance INTO current_balance FROM Accounts WHERE account_number = p_account_number FOR UPDATE;
  
  IF current_balance >= p_amount THEN
    UPDATE Accounts SET balance = balance - p_amount WHERE account_number = p_account_number;
    INSERT INTO Transactions (account_number, transaction_type, amount) VALUES (p_account_number, 'Debit', p_amount);
  ELSE
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'Insufficient funds'; -- 45000(data access errors
  END IF;
END //
DELIMITER ;



-- DELIMITER //
-- -- d) Check Balance
-- CREATE PROCEDURE CheckBalance (
--   IN account_number INT,
--   OUT balance DECIMAL(10,2)
-- )

-- BEGIN
--   SELECT balance INTO balance FROM Accounts WHERE account_number = account_number;
-- END //

-- -- 4. Set delimiter back to semicolon
-- DELIMITER ;


-- SET SQL_SAFE_UPDATES = 0;

 -- GRANT EXECUTE ON PROCEDURE
--  DebitAccount TO 'employee'@'localhost';

-- GRANT UPDATE ON sys.Accounts TO 'user'@'host' WITH GRANT OPTION;

--  SHOW GRANTS FOR CURRENT_USER();
--  GRANT EXECUTE ON PROCEDURE DebitAccount TO 'DESKTOP-C34M51K'@'localhost';
-- -- -- GRANT ALL PROCEDURE ON PROCEDURE TO 'DESKTOP-C34M51K'@'localhost';
-- -- -- grant execute on [sys].[procedure] to user with grant option;
--  CALL CreateCustomerAccount("krishna",'guntur',888888888);
--  CALL CreditAccount(769312681, 9000.00);


--  CREATE USER 'LAPTOP-IJ6V1L5L''@localhost' IDENTIFIED BY 'SDMS@232004';
--  GRANT EXECUTE ON PROCEDURE DebitAccount TO  'LAPTOP-IJ6V1L5L'@'localhost';

-- CREATE USER 'LAPTOP-IJ6V1L5L'@'localhost' IDENTIFIED BY 'SDMS@232004';

SELECT *FROM Transactions;

SELECT *FROM Customers;
 
SELECT *FROM Accounts;


CALL CreateCustomerAccount("Vi","Gu",9009090988);

CALL CreateCustomerAccount("Vija","Gun",9009090909);
CALL CreateCustomerAccount("sath","hyd",9090900903);
CALL CreateCustomerAccount("hemu","vij",9090990904);
CALL CreateCustomerAccount("jasw","ug",9090990905);
CALL CreateCustomerAccount("deek","viz",909090909);
CALL CreateCustomerAccount("X","viH",909090901);
CALL CreateCustomerAccount("p","ciH",09090907);

select * from customers;
CALL CreditAccount(1,2000.00);
CALL  DebitAccount(1,500.00);
CALL  CheckBalance (4);
CALL DebitAccount(4,500.00);
CALL CreditAccount(6,4000.00);
CALL DebitAccount(5,1000.00);
CALL DebitAccount(5,500.00);    
CALL CreditAccount(5,2000.00);
        
CALL CreateCustomerAccount("V","Hn",900909036);





-- password-SDMS@232004
-- LAPTOP-IJ6V1L5L
