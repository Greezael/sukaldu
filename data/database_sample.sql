-- Product measurement
INSERT INTO prod_meas VALUES (NULL, 'g')
INSERT INTO prod_meas VALUES (NULL, 'uds')
INSERT INTO prod_meas VALUES (NULL, 'l')

-- Product categories
INSERT INTO prod_cat VALUES (1, 'prod_cat1')
INSERT INTO prod_cat VALUES (2, 'prod_cat2')
INSERT INTO prod_cat VALUES (3, 'prod_cat3')
INSERT INTO prod_subcat VALUES (1, 1, 'prod_subcat1')
INSERT INTO prod_subcat VALUES (2, 2, 'prod_subcat2')
INSERT INTO prod_subcat VALUES (3, 2, 'prod_subcat3')
INSERT INTO prod_subcat VALUES (4, 3, 'prod_subcat4')

-- Recipe categories
INSERT INTO recipe_cat VALUES (1, 'recipe_cat1')
INSERT INTO recipe_cat VALUES (2, 'recipe_cat2')
INSERT INTO recipe_cat VALUES (3, 'recipe_cat3')
INSERT INTO recipe_subcat VALUES (1, 1, 'recipe_subcat1')
INSERT INTO recipe_subcat VALUES (2, 2, 'recipe_subcat2')
INSERT INTO recipe_subcat VALUES (3, 2, 'recipe_subcat3')
INSERT INTO recipe_subcat VALUES (4, 1, 'recipe_subcat4')

-- Menu categories
INSERT INTO menu_cat VALUES (1, 'menu_cat1')
INSERT INTO menu_cat VALUES (2, 'menu_cat2')
INSERT INTO menu_cat VALUES (3, 'menu_cat3')
INSERT INTO menu_subcat VALUES (NULL, 1, 'menu_subcat1')
INSERT INTO menu_subcat VALUES (NULL, 2, 'menu_subcat2')
INSERT INTO menu_subcat VALUES (NULL, 2, 'menu_subcat3')
INSERT INTO menu_subcat VALUES (NULL, 1, 'menu_subcat4')

PRAGMA foreign_keys = OFF;
-- Products
INSERT INTO product VALUES (1, 'product1', 1, 1, 'notes', 1, 1)
INSERT INTO product VALUES (2, 'product2', 2, 2, 'notes', 1, 2)
INSERT INTO product VALUES (3, 'product3', 2, 3, 'notes', 2, 3)
INSERT INTO product VALUES (4, 'product4', 2, 3, 'notes', 3, 5)
INSERT INTO product VALUES (5, 'product5', 3, 4, 'notes', 3, NULL)

-- Product prices
INSERT INTO prod_price VALUES (1, 1, 1.5, 1.0, NULL)
INSERT INTO prod_price VALUES (2, 2, 2.5, 1.0, NULL)
INSERT INTO prod_price VALUES (3, 3, 3.5, 1.0, NULL)
INSERT INTO prod_price VALUES (4, 4, 4.5, 1.0, NULL)
INSERT INTO prod_price VALUES (5, 4, 5.5, 1.0, "Another provider")

PRAGMA foreign_keys = ON;

-- Recipes
INSERT INTO recipe VALUES (1, 'recipe1', 1, 1, 'preparation', 1)
INSERT INTO recipe VALUES (2, 'recipe2', 2, 2, 'preparation', 1)
INSERT INTO recipe VALUES (3, 'recipe3', 2, 3, 'preparation', 2)
INSERT INTO recipe VALUES (4, 'recipe4', 3, NULL, 'preparation', 1)

-- Menu
INSERT INTO menu VALUES (1, 'menu1', 1, 1, 'notes')
INSERT INTO menu VALUES (2, 'menu2', 2, 2, 'notes')
INSERT INTO menu VALUES (3, 'menu3', 2, 3, 'notes')
INSERT INTO menu VALUES (4, 'menu4', 3, NULL, 'notes')

-- Recipe is composed of products
INSERT INTO recipe_product VALUES (1, 1, 1.5)
INSERT INTO recipe_product VALUES (1, 2, 1.5)
INSERT INTO recipe_product VALUES (2, 1, 1.5)
INSERT INTO recipe_product VALUES (2, 3, 1.5)
INSERT INTO recipe_product VALUES (3, 3, 1.5)
INSERT INTO recipe_product VALUES (4, 4, 1.2)

-- Menu is composed of recipes
INSERT INTO menu_recipe VALUES (1, 1)
INSERT INTO menu_recipe VALUES (1, 2)
INSERT INTO menu_recipe VALUES (1, 3)
INSERT INTO menu_recipe VALUES (2, 1)
INSERT INTO menu_recipe VALUES (2, 2)
INSERT INTO menu_recipe VALUES (2, 4)
INSERT INTO menu_recipe VALUES (3, 1)
INSERT INTO menu_recipe VALUES (3, 3)
