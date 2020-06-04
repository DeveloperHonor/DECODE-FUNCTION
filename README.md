# DECODE-FUNCTION
In PostgreSQL,achieving similar to decode function of Oracle 

# Step 1 #
**Download decode.c file**

# Step 2 #
**Putting the decode.c to $PGDATA/../include/postgresql/server or src/include directory**

# Step 3#
Executing following command when you completed step 2

cd $PGDATA/../include/postgresql/server/

gcc -fpic -c decode.c -I .   #don't forget the point

gcc -shared -o decode.so decode.o

cp decode.so $PGDATA/../lib/

# Step 4#
**Download decode.sql file**

put the decode.sql file in /home/postgres directory or other directory

Next,as following:
psql -U postgres -d postgres -f decode.sql

# Verify #

    postgres=# SELECT decode(1,1,'PostgreSQL',2,'MySQL','Other');
       decode   
    ------------
     PostgreSQL
    (1 row)




