SELECT o.officecode,
       Sum(o2.quantityordered) AS "Unidades vendidas"
FROM   offices o,
       employees e,
       customers c,
       orders o1,
       orderdetails o2
WHERE  o.officecode = e.officecode
       AND c.salesrepemployeenumber = e.employeenumber
       AND o1.customernumber = c.customernumber
       AND o2.ordernumber = o1.ordernumber
GROUP  BY o.officecode
ORDER  BY "Unidades vendidas" DESC
LIMIT  1;