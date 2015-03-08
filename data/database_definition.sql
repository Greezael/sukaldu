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
product INTEGER,
price REAL,
quantity REAL,
notes TEXT,
FOREIGN KEY (product) references product ON DELETE CASCADE
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
recipe INTEGER,
product INTEGER,
quantity REAL,
PRIMARY KEY (recipe, product),
FOREIGN KEY (product) references product ON DELETE CASCADE,
FOREIGN KEY (recipe) references recipe ON DELETE CASCADE
)

-- Menu is composed of recipes
CREATE TABLE menu_recipe (
menu INTEGER,
recipe INTEGER,
PRIMARY KEY (menu, recipe),
FOREIGN KEY (menu) references menu ON DELETE CASCADE,
FOREIGN KEY (recipe) references recipe ON DELETE CASCADE
)

-- COMPUTED
-- Product prices
CREATE VIEW C_prod_price AS
SELECT product.id 'id', prod_price.price 'price'
FROM product LEFT JOIN prod_price
ON product.current_price = prod_price.id

-- COMPUTED
-- Recipe prices
CREATE VIEW C_recipe_price AS
SELECT recipe.id, (SUM(price * quantity)/servings) 'price'
FROM recipe LEFT JOIN recipe_product
ON recipe.id = recipe_product.recipe
LEFT JOIN C_prod_price
ON recipe_product.product = C_prod_price.id
GROUP BY recipe.id

-- COMPUTED
-- Menu prices
CREATE VIEW C_menu_price AS
SELECT menu.id, SUM(price) 'price'
FROM menu LEFT JOIN menu_recipe
ON menu.id = menu_recipe.menu
LEFT JOIN C_recipe_price
ON menu_recipe.recipe = C_recipe_price.id
GROUP BY menu.id

