--Este programa considera venta solo si el estado del producto es Shipped

SELECT country,
       Count(*) AS "Número de oficinas"
FROM   offices o
WHERE  officecode NOT IN (SELECT DISTINCT officecode
                          FROM   employees,
                                 customers c
                          WHERE  c.customernumber IN
                                 (SELECT DISTINCT customernumber
                                  FROM   orders
                                  WHERE  orderdate >= '2003-01-01'
                                         AND orderdate <=
                                             '2003-12-31'
                                         AND status = 'Shipped')
                                 AND c.salesrepemployeenumber = employeenumber)
GROUP  BY country
ORDER  BY "Número de oficinas" DESC;

