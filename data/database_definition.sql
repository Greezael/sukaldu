-- Product measurement
CREATE TABLE prod_meas (
id INT AUTO_INCREMENT PRIMARY KEY,
name VARCHAR(100)
)

-- Product prices
CREATE TABLE prod_price (
id INT AUTO_INCREMENT PRIMARY KEY,
product INT REFERENCES product,
price REAL,
notes TEXT
)

-- Product categories
CREATE TABLE prod_cat (
id INT AUTO_INCREMENT PRIMARY KEY,
name VARCHAR(100)
)

CREATE TABLE prod_subcat (
id INT AUTO_INCREMENT PRIMARY KEY,
cat INT REFERENCES prod_cat,
name VARCHAR(100)
)


-- Recipe categories
CREATE TABLE recipe_cat (
id INT AUTO_INCREMENT PRIMARY KEY,
name VARCHAR(100)
)

CREATE TABLE recipe_subcat (
id INT AUTO_INCREMENT PRIMARY KEY,
cat INT REFERENCES recipe_cat,
name VARCHAR(100)
)


-- Menu categories
CREATE TABLE menu_cat (
id INT AUTO_INCREMENT PRIMARY KEY,
name VARCHAR(100)
)

CREATE TABLE menu_subcat (
id INT AUTO_INCREMENT PRIMARY KEY,
cat INT REFERENCES menu_cat,
name VARCHAR(100)
)

-- Products
CREATE TABLE product (
id INT AUTO_INCREMENT PRIMARY KEY,
name VARCHAR(100),
cat INT REFERENCES prod_cat,
subcat INT REFERENCES prod_subcat,
notes TEXT,
meas INT REFERENCES prod_meas,
current_price INT REFERENCES prod_price
)

-- Recipes
CREATE TABLE recipe (
id INT AUTO_INCREMENT PRIMARY KEY,
name VARCHAR(100),
cat INT REFERENCES recipe_cat,
subcat INT REFERENCES recipe_subcat,
preparation TEXT,
servings INT
)

-- Menu
CREATE TABLE menu (
id INT AUTO_INCREMENT PRIMARY KEY,
name VARCHAR(100),
cat INT REFERENCES menu_cat,
subcat INT REFERENCES menu_subcat,
notes TEXT
)

-- Recipe is composed of products
CREATE TABLE recipe_product (
recipe INT REFERENCES recipe,
product INT REFERENCES product,
quantity REAL,
PRIMARY KEY (recipe, product)
)

-- Menu is composed of recipes
CREATE TABLE menu_recipe (
menu INT REFERENCES menu,
recipe INT REFERENCES recipe,
PRIMARY KEY (menu, recipe)
)
