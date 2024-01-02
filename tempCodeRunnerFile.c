for(int i=0; transactions[i].date != 0 ;i++){
        if(i != lineToDelete-1) {
            fprintf(file, "%d,%s,%s,%.2f\n", transactions[i].date, transactions[i].type, transactions[i].method, 
            transactions[i].value);
        }
    