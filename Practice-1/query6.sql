WITH lista
     AS (SELECT o.productcode  AS producto1,
                o2.productcode AS producto2,
                o.ordernumber
         FROM   orderdetails o,
                orderdetails o2
         WHERE  ( o.ordernumber = o2.ordernumber
                  AND o.productcode < o2.productcode ))
SELECT producto1,
       producto2,
       Count(*) AS "Carros de Compra"
FROM   lista
GROUP  BY producto1,
          producto2
HAVING Count(*) > 1
ORDER  BY "Carros de Compra" ASC;