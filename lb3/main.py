def levenshtein_distance_with_steps(s, t):
    m = len(s)
    n = len(t)
    
    if m < n:
        return levenshtein_distance_with_steps(t, s)
    
    # DP table to store distances
    dp = [[0] * (n + 1) for _ in range(m + 1)]
    # Operation table to store operations
    ops = [[''] * (n + 1) for _ in range(m + 1)]
    
    # Initialize the first row and column
    for j in range(n + 1):
        dp[0][j] = j
        ops[0][j] = 'I' * j  # Insertions to build t[:j]
    for i in range(m + 1):
        dp[i][0] = i
        ops[i][0] = 'D' * i  # Deletions to reduce s[:i] to empty
    
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if s[i - 1] == t[j - 1]:
                dp[i][j] = dp[i - 1][j - 1]
                ops[i][j] = ops[i - 1][j - 1] + 'M'  # Match
            else:
                # Possible operations and their costs
                delete = dp[i - 1][j] + 1
                insert = dp[i][j - 1] + 1
                substitute = dp[i - 1][j - 1] + 1
                
                # Find the minimum cost operation
                if delete <= insert and delete <= substitute:
                    dp[i][j] = delete
                    ops[i][j] = ops[i - 1][j] + 'D'  # Deletion
                elif insert <= delete and insert <= substitute:
                    dp[i][j] = insert
                    ops[i][j] = ops[i][j - 1] + 'I'  # Insertion
                else:
                    dp[i][j] = substitute
                    ops[i][j] = ops[i - 1][j - 1] + 'S'  # Substitution
    
    # Backtrack to get the sequence of operations
    operations = []
    i, j = m, n
    while i > 0 or j > 0:
        if i > 0 and j > 0 and s[i - 1] == t[j - 1]:
            operations.append('M')  # Match
            i -= 1
            j -= 1
        else:
            if i > 0 and dp[i][j] == dp[i - 1][j] + 1:
                operations.append('D')  # Deletion
                i -= 1
            elif j > 0 and dp[i][j] == dp[i][j - 1] + 1:
                operations.append('I')  # Insertion
                j -= 1
            elif i > 0 and j > 0 and dp[i][j] == dp[i - 1][j - 1] + 1:
                operations.append('S')  # Substitution
                i -= 1
                j -= 1
    
    operations.reverse()  # Reverse to get the correct order
    
    # Print the operations and steps
    print("Operations:", ''.join(operations))
    current = list(s)
    ptr_s = 0
    ptr_t = 0
    steps = []
    for op in operations:
        if op == 'M':
            steps.append(f"Match '{s[ptr_s]}'")
            ptr_s += 1
            ptr_t += 1
        elif op == 'D':
            steps.append(f"Delete '{s[ptr_s]}'")
            del current[ptr_s]
        elif op == 'I':
            steps.append(f"Insert '{t[ptr_t]}'")
            current.insert(ptr_s, t[ptr_t])
            ptr_s += 1
            ptr_t += 1
        elif op == 'S':
            steps.append(f"Substitute '{s[ptr_s]}' with '{t[ptr_t]}'")
            current[ptr_s] = t[ptr_t]
            ptr_s += 1
            ptr_t += 1
        steps.append("Current string: " + ''.join(current))
    
    print("\nSteps:")
    for step in steps:
        print(step)
    
    return dp[m][n]

s = input().strip()
t = input().strip()
distance = levenshtein_distance_with_steps(s, t)
print("\nLevenshtein distance:", distance)