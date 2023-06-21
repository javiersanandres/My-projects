--
-- PostgreSQL database dump
--

-- Dumped from database version 12.12 (Ubuntu 12.12-0ubuntu0.20.04.1)
-- Dumped by pg_dump version 12.12 (Ubuntu 12.12-0ubuntu0.20.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'LATIN9';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: customers; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.customers (
    customernumber integer NOT NULL,
    customername character varying(50) NOT NULL,
    contactlastname character varying(50) NOT NULL,
    contactfirstname character varying(50) NOT NULL,
    phone character varying(50) NOT NULL,
    addressline1 character varying(50) NOT NULL,
    addressline2 character varying(50) DEFAULT NULL::character varying,
    city character varying(50) NOT NULL,
    state character varying(50) DEFAULT NULL::character varying,
    postalcode character varying(15) DEFAULT NULL::character varying,
    country character varying(50) NOT NULL,
    creditlimit numeric(10,2) DEFAULT NULL::numeric
);


ALTER TABLE public.customers OWNER TO alumnodb;

--
-- Name: employees; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.employees (
    employeenumber integer NOT NULL,
    lastname character varying(50) NOT NULL,
    firstname character varying(50) NOT NULL,
    extension character varying(10) NOT NULL,
    email character varying(100) NOT NULL,
    officecode character varying(10) NOT NULL,
    reportsto integer,
    jobtitle character varying(50) NOT NULL
);


ALTER TABLE public.employees OWNER TO alumnodb;

--
-- Name: empofficehistory; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.empofficehistory (
    employeenumber integer NOT NULL,
    transitiondate date NOT NULL,
    oldofficecode character varying(10),
    nextofficecode character varying(10) NOT NULL
);


ALTER TABLE public.empofficehistory OWNER TO alumnodb;

--
-- Name: offices; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.offices (
    officecode character varying(10) NOT NULL,
    city character varying(50) NOT NULL,
    phone character varying(50) NOT NULL,
    addressline1 character varying(50) NOT NULL,
    addressline2 character varying(50) DEFAULT NULL::character varying,
    state character varying(50) DEFAULT NULL::character varying,
    country character varying(50) NOT NULL,
    postalcode character varying(15) NOT NULL,
    territory character varying(10) NOT NULL
);


ALTER TABLE public.offices OWNER TO alumnodb;

--
-- Name: orderdetails; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.orderdetails (
    ordernumber integer NOT NULL,
    productcode character varying(15) NOT NULL,
    quantityordered integer NOT NULL,
    priceeach numeric(10,2) NOT NULL,
    orderlinenumber smallint NOT NULL
);


ALTER TABLE public.orderdetails OWNER TO alumnodb;

--
-- Name: orders; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.orders (
    ordernumber integer NOT NULL,
    orderdate date NOT NULL,
    requireddate date NOT NULL,
    shippeddate date,
    status character varying(15) NOT NULL,
    comments text,
    customernumber integer NOT NULL,
    salesrepemployeenumber integer
);


ALTER TABLE public.orders OWNER TO alumnodb;

--
-- Name: payments; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.payments (
    ordernumber integer NOT NULL,
    checknumber character varying(50) NOT NULL,
    paymentdate date NOT NULL,
    amount numeric(10,2) NOT NULL
);


ALTER TABLE public.payments OWNER TO alumnodb;

--
-- Name: productlines; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.productlines (
    productline character varying(50) NOT NULL,
    textdescription character varying(4000) DEFAULT NULL::character varying,
    htmldescription character varying(4000),
    image character varying
);


ALTER TABLE public.productlines OWNER TO alumnodb;

--
-- Name: products; Type: TABLE; Schema: public; Owner: alumnodb
--

CREATE TABLE public.products (
    productcode character varying(15) NOT NULL,
    productname character varying(70) NOT NULL,
    productline character varying(50) NOT NULL,
    productscale character varying(10) NOT NULL,
    productvendor character varying(50) NOT NULL,
    productdescription text NOT NULL,
    quantityinstock smallint NOT NULL,
    buyprice numeric(10,2) NOT NULL,
    msrp numeric(10,2) NOT NULL
);


ALTER TABLE public.products OWNER TO alumnodb;

--
-- Data for Name: customers; Type: TABLE DATA; Schema: public; Owner: alumnodb
--

COPY public.customers (customernumber, customername, contactlastname, contactfirstname, phone, addressline1, addressline2, city, state, postalcode, country, creditlimit) FROM stdin;
\.


--
-- Data for Name: employees; Type: TABLE DATA; Schema: public; Owner: alumnodb
--

COPY public.employees (employeenumber, lastname, firstname, extension, email, officecode, reportsto, jobtitle) FROM stdin;
\.


--
-- Data for Name: empofficehistory; Type: TABLE DATA; Schema: public; Owner: alumnodb
--

COPY public.empofficehistory (employeenumber, transitiondate, oldofficecode, nextofficecode) FROM stdin;
\.


--
-- Data for Name: offices; Type: TABLE DATA; Schema: public; Owner: alumnodb
--

COPY public.offices (officecode, city, phone, addressline1, addressline2, state, country, postalcode, territory) FROM stdin;
\.


--
-- Data for Name: orderdetails; Type: TABLE DATA; Schema: public; Owner: alumnodb
--

COPY public.orderdetails (ordernumber, productcode, quantityordered, priceeach, orderlinenumber) FROM stdin;
\.


--
-- Data for Name: orders; Type: TABLE DATA; Schema: public; Owner: alumnodb
--

COPY public.orders (ordernumber, orderdate, requireddate, shippeddate, status, comments, customernumber, salesrepemployeenumber) FROM stdin;
\.


--
-- Data for Name: payments; Type: TABLE DATA; Schema: public; Owner: alumnodb
--

COPY public.payments (ordernumber, checknumber, paymentdate, amount) FROM stdin;
\.


--
-- Data for Name: productlines; Type: TABLE DATA; Schema: public; Owner: alumnodb
--

COPY public.productlines (productline, textdescription, htmldescription, image) FROM stdin;
\.


--
-- Data for Name: products; Type: TABLE DATA; Schema: public; Owner: alumnodb
--

COPY public.products (productcode, productname, productline, productscale, productvendor, productdescription, quantityinstock, buyprice, msrp) FROM stdin;
\.


--
-- Name: customers customers_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.customers
    ADD CONSTRAINT customers_pkey PRIMARY KEY (customernumber);


--
-- Name: employees employees_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.employees
    ADD CONSTRAINT employees_pkey PRIMARY KEY (employeenumber);


--
-- Name: empofficehistory empofficehistory_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.empofficehistory
    ADD CONSTRAINT empofficehistory_pkey PRIMARY KEY (employeenumber, transitiondate);


--
-- Name: offices offices_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.offices
    ADD CONSTRAINT offices_pkey PRIMARY KEY (officecode);


--
-- Name: orderdetails orderdetails_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orderdetails
    ADD CONSTRAINT orderdetails_pkey PRIMARY KEY (ordernumber, productcode);


--
-- Name: orders orders_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orders
    ADD CONSTRAINT orders_pkey PRIMARY KEY (ordernumber);


--
-- Name: payments payments_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.payments
    ADD CONSTRAINT payments_pkey PRIMARY KEY (ordernumber, checknumber);


--
-- Name: productlines productlines_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.productlines
    ADD CONSTRAINT productlines_pkey PRIMARY KEY (productline);


--
-- Name: products products_pkey; Type: CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.products
    ADD CONSTRAINT products_pkey PRIMARY KEY (productcode);


--
-- Name: customernumber; Type: INDEX; Schema: public; Owner: alumnodb
--

CREATE INDEX customernumber ON public.orders USING btree (customernumber);


--
-- Name: officecode; Type: INDEX; Schema: public; Owner: alumnodb
--

CREATE INDEX officecode ON public.employees USING btree (officecode);


--
-- Name: productcode; Type: INDEX; Schema: public; Owner: alumnodb
--

CREATE INDEX productcode ON public.orderdetails USING btree (productcode);


--
-- Name: reportsto; Type: INDEX; Schema: public; Owner: alumnodb
--

CREATE INDEX reportsto ON public.employees USING btree (reportsto);


--
-- Name: employees employees_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.employees
    ADD CONSTRAINT employees_ibfk_1 FOREIGN KEY (reportsto) REFERENCES public.employees(employeenumber);


--
-- Name: employees employees_ibfk_2; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.employees
    ADD CONSTRAINT employees_ibfk_2 FOREIGN KEY (officecode) REFERENCES public.offices(officecode);


--
-- Name: empofficehistory employees_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.empofficehistory
    ADD CONSTRAINT employees_ibfk_1 FOREIGN KEY (employeenumber) REFERENCES public.employees(employeenumber);

--
-- Name: empofficehistory employees_ibfk_2; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.empofficehistory
    ADD CONSTRAINT employees_ibfk_2 FOREIGN KEY (oldofficecode) REFERENCES public.offices(officecode);

--
-- Name: empofficehistory employees_ibfk_3; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.empofficehistory
    ADD CONSTRAINT employees_ibfk_3 FOREIGN KEY (nextofficecode) REFERENCES public.offices(officecode);


--
-- Name: orderdetails orderdetails_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orderdetails
    ADD CONSTRAINT orderdetails_ibfk_1 FOREIGN KEY (ordernumber) REFERENCES public.orders(ordernumber);


--
-- Name: orderdetails orderdetails_ibfk_2; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orderdetails
    ADD CONSTRAINT orderdetails_ibfk_2 FOREIGN KEY (productcode) REFERENCES public.products(productcode);


--
-- Name: orders orders_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orders
    ADD CONSTRAINT orders_ibfk_1 FOREIGN KEY (customernumber) REFERENCES public.customers(customernumber);


--
-- Name: orders orders_ibfk_2; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.orders
    ADD CONSTRAINT orders_ibfk_2 FOREIGN KEY (salesrepemployeenumber) REFERENCES public.employees(employeenumber);


--
-- Name: payments payments_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.payments
    ADD CONSTRAINT payments_ibfk_1 FOREIGN KEY (ordernumber) REFERENCES public.orders(ordernumber);


--
-- Name: products products_ibfk_1; Type: FK CONSTRAINT; Schema: public; Owner: alumnodb
--

ALTER TABLE ONLY public.products
    ADD CONSTRAINT products_ibfk_1 FOREIGN KEY (productline) REFERENCES public.productlines(productline);


--
-- PostgreSQL database dump complete
--

