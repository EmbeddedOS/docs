# Splunk zero to power user

## 15. transforming your search

- Top 3 common commands:
  - `top`: Finds the top common values of a field in a table.
  - `rare`: find the least common values of a field in a table.
  - `stats`: calculate statistics: `count`, `dc`, `sum`, `avg`, `list`, `values`, etc.
    - `dc`: how many distinct values.


`| stats count by src, user, action` -> count by group of 3. Every different value in one of them make a new row.

For example:

```text
|   src         |   user    |       action      | count
| 10.1.10.172   |    lyn    |       success     | 122
| 10.1.10.172   |    rose   |       success     | 12
| 10.1.10.171   |    lyn    |       failure     | 8
```


`stats values(user) as "Login Name", count(user) as "Attempts" by src` the `values()` function here collect all value for one special source and put it inside an item.

For example:

```text
|   src         |   Login Name      | Attempts
| 10.1.10.172   |  lava             | 122
|               |  susan            |
|               |  lyn              |
|               |  xuka             |
| 10.1.10.171   |  lava             | 8
|               |  meta             |
| 10.1.10.170   |  lina             | 2
```

So now the Login Name column value can contain multiple values of `user`.

`fillnull value="no data available"` Some `src` with no count (no `user`) can be display as `no data available`.

```text
|   src         |   Login Name      | Attempts
| 10.1.10.172   |  lava             | 122
|               |  susan            |
|               |  lyn              |
|               |  xuka             |
| 10.1.10.171   |  lava             | 8
|               |  meta             |
| 10.1.10.170   |  lina             | 2
| 10.1.10.169   | no data available | 0
```

## 17. What are the events telling me?

- `transaction` command:
  - `maxspan`: Max time between all related events.
  - `maxpause`: Max time between each individual event.
  - `startswith` and `endswith`: Set your variables Windows EventIDs.
- Investigate your events:
- Transaction vs `stats`:

## 18. Manipulating your data

- Calculates fields: Does the math you ask: +, -, *, /, AND, XOR, >=, <=, etc.
- Functions friendly: Just like stats, it takes plenty of functional arguments: if, null, true, cidmatch, like, lookup, tostring, md5, now, strtime.
- Create new fields: `eval` will take the results of the SPL into a field that is existing, or create a new one.
- Converting data: Tell splunk to display a field value of bytes to megabytes by providing the math in an `eval` statement `strftime`, `strptime`.

## 19. Demo eval, search, where



Example of `eval` with case() func: `| eval status_code = case((status == 404), "not found", (status == 400), "bad request")`

Normally you do filter in the first of SPL query. For example

```text
Event=MyEvent Code<100
| <some commands>
| <visualize commands>
```

But sometimes, you want to filter after a few commands (because you filter based on some variable you just calculated). To do that you can use `where` command. For example:

```text
index=security
| table src, user, action
| where like(src, "10.1.10.%")
| where user=lava
```

The output can be filter:

```text
|   src         |   user    |       action
| 10.1.10.172   |    lava   |       success
| 10.1.10.171   |    lava   |       failure
```

## 20. Fields part 2

## 26. Demo visualize your data with chart and stats
