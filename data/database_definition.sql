PRAGMA foreign_keys = ON;

-- Database info
CREATE TABLE sukaldu_db (
infokey VARCHAR(100) PRIMARY KEY,
infovalue VARCHAR(100)
)


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
cat INTEGER NOT NULL,
name VARCHAR(100),
FOREIGN KEY (cat) references prod_cat ON DELETE CASCADE
)


-- Recipe categories
CREATE TABLE recipe_cat (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100)
)

CREATE TABLE recipe_subcat (
id INTEGER PRIMARY KEY AUTOINCREMENT,
cat INTEGER NOT NULL,
name VARCHAR(100),
FOREIGN KEY (cat) references recipe_cat ON DELETE CASCADE
)


-- Menu categories
CREATE TABLE menu_cat (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100)
)

CREATE TABLE menu_subcat (
id INTEGER PRIMARY KEY AUTOINCREMENT,
cat INTEGER NOT NULL,
name VARCHAR(100),
FOREIGN KEY (cat) references menu_cat ON DELETE CASCADE
)

-- Products
CREATE TABLE product (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100),
cat INTEGER,
subcat INTEGER,
notes TEXT,
meas INTEGER,
current_price INTEGER,
FOREIGN KEY (current_price) references prod_price ON DELETE SET NULL,
FOREIGN KEY (cat) references prod_cat ON DELETE SET NULL,
FOREIGN KEY (subcat) references prod_subcat ON DELETE SET NULL,
FOREIGN KEY (meas) references prod_meas ON DELETE SET NULL
)

-- Price provider
CREATE TABLE price_provider (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100)
)

-- Product prices
CREATE TABLE prod_price (
id INTEGER PRIMARY KEY AUTOINCREMENT,
product INTEGER,
price REAL,
quantity REAL,
provider INTEGER,
notes TEXT,
FOREIGN KEY (product) references product ON DELETE CASCADE,
FOREIGN KEY (provider) references price_provider ON DELETE CASCADE
)

-- Recipes
CREATE TABLE recipe (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100),
cat INTEGER,
subcat INTEGER,
preparation TEXT,
servings INTEGER,
FOREIGN KEY (cat) references recipe_cat ON DELETE SET NULL,
FOREIGN KEY (subcat) references recipe_subcat ON DELETE SET NULL
)

-- Menu
CREATE TABLE menu (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name VARCHAR(100),
cat INTEGER,
subcat INTEGER,
notes TEXT,
FOREIGN KEY (cat) references menu_cat ON DELETE SET NULL,
FOREIGN KEY (subcat) references menu_subcat ON DELETE SET NULL
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

-- Menu / recipe composition can have different roles
CREATE TABLE menu_role (
id INTEGER PRIMARY KEY AUTOINCREMENT,
menu INTEGER,
name VARCHAR(100),
FOREIGN KEY (menu) references menu ON DELETE CASCADE
)

-- Menu is composed of recipes
CREATE TABLE menu_recipe (
menu INTEGER,
recipe INTEGER,
role INTEGER,
PRIMARY KEY (menu, recipe, role),
FOREIGN KEY (menu) references menu ON DELETE CASCADE,
FOREIGN KEY (recipe) references recipe ON DELETE CASCADE,
FOREIGN KEY (role) references menu_role ON DELETE CASCADE
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
-- Menu Role prices
CREATE VIEW C_role_price AS
SELECT menu_role.menu, menu_role.id,
    AVG(price) 'avg_price', MAX(price) 'max_price', MIN(price) 'min_price'
FROM menu_role
LEFT JOIN menu_recipe
    ON menu_role.id = menu_recipe.role
LEFT JOIN C_recipe_price
    ON menu_recipe.recipe = C_recipe_price.id
GROUP BY menu_role.id

-- COMPUTED
-- Menu prices
CREATE VIEW C_menu_price AS
SELECT menu.id, SUM(avg_price) 'avg_price', SUM(min_price) 'min_price', SUM(max_price) 'max_price'
FROM menu LEFT JOIN C_role_price
ON menu.id = C_role_price.menu
GROUP BY menu.id

