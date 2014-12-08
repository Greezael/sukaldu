-- Product measurement

CREATE TABLE prod_meas {
id INT PRIMARY KEY,
name VARCHAR(100)
}

-- Product categories

CREATE TABLE prod_cat (
id INT PRIMARY KEY,
name VARCHAR(100)
)

CREATE TABLE prod_subcat (
id INT PRIMARY KEY,
cat INT REFERENCES prod_cat,
name VARCHAR(100)
)


-- Recipe categories

CREATE TABLE recipe_cat (
id INT PRIMARY KEY,
name VARCHAR(100)
)

CREATE TABLE recipe_subcat (
id INT PRIMARY KEY,
cat INT REFERENCES recipe_cat,
name VARCHAR(100)
)


-- Menu categories

CREATE TABLE menu_cat (
id INT PRIMARY KEY,
name VARCHAR(100)
)

CREATE TABLE menu_subcat (
id INT PRIMARY KEY,
cat INT REFERENCES menu_cat,
name VARCHAR(100)
)


-- Products

CREATE TABLE product (
id INT PRIMARY KEY,
name VARCHAR(100),
cat INT REFERENCES prod_cat,
subcat INT REFERENCES prod_subcat,
notes TEXT,
meas INT REFERENCES prod_meas
)
