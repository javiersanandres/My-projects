WITH Pickup_customers
     AS (SELECT DISTINCT c.customernumber,
                         c.customername
         FROM   customers c,
                orders o1,
                orderdetails o2,
                products p
         WHERE  p.productname = '1940 Ford Pickup Truck'
                AND o2.productcode = p.productcode
                AND o2.ordernumber = o1.ordernumber
                AND o1.customernumber = c.customernumber)
SELECT p.customernumber,
       p.customername,
       Sum(pa.amount) AS "Total Pagado"
FROM   Pickup_customers p,
       payments pa
WHERE  p.customernumber = pa.customernumber
GROUP  BY p.customernumber,
          p.customername
ORDER  BY "Total Pagado" DESC;