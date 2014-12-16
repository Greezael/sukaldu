-- Product measurement
INSERT INTO prod_meas VALUES (NULL, 'g')
INSERT INTO prod_meas VALUES (NULL, 'uds')
INSERT INTO prod_meas VALUES (NULL, 'l')

-- Product categories
INSERT INTO prod_cat VALUES (1, 'prod_cat1')
INSERT INTO prod_cat VALUES (2, 'prod_cat2')
INSERT INTO prod_cat VALUES (3, 'prod_cat3')
INSERT INTO prod_subcat VALUES (NULL, 1, 'prod_subcat1')
INSERT INTO prod_subcat VALUES (NULL, 2, 'prod_subcat2')
INSERT INTO prod_subcat VALUES (NULL, 2, 'prod_subcat3')
INSERT INTO prod_subcat VALUES (NULL, 1, 'prod_subcat4')

-- Product categories
INSERT INTO recipe_cat VALUES (1, 'recipe_cat1')
INSERT INTO recipe_cat VALUES (2, 'recipe_cat2')
INSERT INTO recipe_cat VALUES (3, 'recipe_cat3')
INSERT INTO recipe_subcat VALUES (NULL, 1, 'recipe_subcat1')
INSERT INTO recipe_subcat VALUES (NULL, 2, 'recipe_subcat2')
INSERT INTO recipe_subcat VALUES (NULL, 2, 'recipe_subcat3')
INSERT INTO recipe_subcat VALUES (NULL, 1, 'recipe_subcat4')

-- Product categories
INSERT INTO menu_cat VALUES (1, 'menu_cat1')
INSERT INTO menu_cat VALUES (2, 'menu_cat2')
INSERT INTO menu_cat VALUES (3, 'menu_cat3')
INSERT INTO menu_subcat VALUES (NULL, 1, 'menu_subcat1')
INSERT INTO menu_subcat VALUES (NULL, 2, 'menu_subcat2')
INSERT INTO menu_subcat VALUES (NULL, 2, 'menu_subcat3')
INSERT INTO menu_subcat VALUES (NULL, 1, 'menu_subcat4')

-- Products
INSERT INTO product VALUES (1, 'product1', 1, 1, 'notes', 1, 1)
INSERT INTO product VALUES (2, 'product2', 2, 2, 'notes', 1, 2)
INSERT INTO product VALUES (3, 'product3', 2, 3, 'notes', 2, 3)
INSERT INTO product VALUES (4, 'product4', 2, 3, 'notes', 3, 4)

-- Product prices
INSERT INTO prod_price VALUES (1, 1, 1.5, NULL)
INSERT INTO prod_price VALUES (2, 2, 2.5, NULL)
INSERT INTO prod_price VALUES (3, 3, 3.5, NULL)
INSERT INTO prod_price VALUES (4, 4, 4.5, NULL)
INSERT INTO prod_price VALUES (5, 4, 5.5, "Another provider")
