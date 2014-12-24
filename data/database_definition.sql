PRAGMA foreign_keys = ON;

-- Product measurement
CREATE TABLE prod_meas (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100)
)

-- Product categories
CREATE TABLE prod_cat (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100)
)

CREATE TABLE prod_subcat (
id INTEGER PRIMARY KEY AUTOINCREMENT,
cat INTEGER NOT NULL REFERENCES prod_cat(id),
name VARCHAR(100)
)


-- Recipe categories
CREATE TABLE recipe_cat (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100)
)

CREATE TABLE recipe_subcat (
id INTEGER PRIMARY KEY AUTOINCREMENT,
cat INTEGER REFERENCES recipe_cat,
name VARCHAR(100)
)


-- Menu categories
CREATE TABLE menu_cat (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100)
)

CREATE TABLE menu_subcat (
id INTEGER PRIMARY KEY AUTOINCREMENT,
cat INTEGER REFERENCES menu_cat,
name VARCHAR(100)
)

-- Products
CREATE TABLE product (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100),
cat INTEGER REFERENCES prod_cat,
subcat INTEGER REFERENCES prod_subcat,
notes TEXT,
meas INTEGER REFERENCES prod_meas,
current_price INTEGER REFERENCES prod_price
)

-- Product prices
CREATE TABLE prod_price (
id INTEGER PRIMARY KEY AUTOINCREMENT,
product INTEGER REFERENCES product,
price REAL,
notes TEXT
)

-- Recipes
CREATE TABLE recipe (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100),
cat INTEGER REFERENCES recipe_cat,
subcat INTEGER REFERENCES recipe_subcat,
preparation TEXT,
servings INTEGER
)

-- Menu
CREATE TABLE menu (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100),
cat INTEGER REFERENCES menu_cat,
subcat INTEGER REFERENCES menu_subcat,
notes TEXT
)

-- Recipe is composed of products
CREATE TABLE recipe_product (
recipe INTEGER REFERENCES recipe,
product INTEGER REFERENCES product,
quantity REAL,
PRIMARY KEY (recipe, product)
)

-- Menu is composed of recipes
CREATE TABLE menu_recipe (
menu INTEGER REFERENCES menu,
recipe INTEGER REFERENCES recipe,
PRIMARY KEY (menu, recipe)
)
