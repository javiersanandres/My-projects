SELECT p.productline,
       Cast(Avg(o1.shippeddate - o1.orderdate) AS DECIMAL(5, 3)) AS
       "Tiempo Medio Transcurrido"
FROM   orders o1,
       orderdetails o2,
       products p
WHERE  o1.ordernumber = o2.ordernumber
       AND o2.productcode = p.productcode
       AND o1.status = 'Shipped'
GROUP  BY p.productline;  
