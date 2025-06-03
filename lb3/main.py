def levenshtein_distance_with_steps(s, t):
    m = len(s)
    n = len(t)
    
    if m < n:
        return levenshtein_distance_with_steps(t, s)
    
    # DP table to store distances
    dp = [[0] * (n + 1) for _ in range(m + 1)]
    # Operation table to store operations
    ops = [[''] * (n + 1) for _ in range(m + 1)]
    
    print("\nInitializing DP table:")
    # Initialize the first row and column
    for j in range(n + 1):
        dp[0][j] = j
        ops[0][j] = 'I' * j  # Insertions to build t[:j]
    for i in range(m + 1):
        dp[i][0] = i
        ops[i][0] = 'D' * i  # Deletions to reduce s[:i] to empty
    
    print("\nInitial DP table:")
    print_dp_table(dp, s, t)
    
    print("\nFilling DP table:")
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
                
                # Initialize double delete with infinity (impossible by default)
                double_delete = float('inf')
                
                # Check if double delete is possible (i > 1 and characters match condition)
                if i > 1 and (i == 2 or s[i-2] == s[i-1]):
                    double_delete = dp[i - 2][j] + 1.5  # Cost for double delete
                
                # Find the minimum cost operation
                min_cost = min(delete, insert, substitute, double_delete)
                dp[i][j] = min_cost
                
                if min_cost == delete:
                    ops[i][j] = ops[i - 1][j] + 'D'  # Single deletion
                elif min_cost == insert:
                    ops[i][j] = ops[i][j - 1] + 'I'  # Insertion
                elif min_cost == substitute:
                    ops[i][j] = ops[i - 1][j - 1] + 'S'  # Substitution
                else:
                    ops[i][j] = ops[i - 2][j] + 'DD'  # Double deletion
            
            print(f"\nAfter processing cell ({i},{j}):")
            print_dp_table(dp, s, t)
            print(f"Operation: {ops[i][j]}")
    
    # Backtrack to get the sequence of operations
    operations = []
    i, j = m, n
    while i > 0 or j > 0:
        if i > 0 and j > 0 and s[i - 1] == t[j - 1]:
            operations.append('M')  # Match
            i -= 1
            j -= 1
        else:
            # Check double delete first (since it's our new operation)
            if i > 1 and dp[i][j] == dp[i - 2][j] + 1.5 and (i == 2 or s[i-2] == s[i-1]):
                operations.append('DD')  # Double deletion
                i -= 2
            elif i > 0 and dp[i][j] == dp[i - 1][j] + 1:
                operations.append('D')  # Single deletion
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
    print("\nFinal Operations:", ''.join(operations))
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
        elif op == 'DD':
            steps.append(f"Double delete '{s[ptr_s]}' and '{s[ptr_s+1]}'")
            del current[ptr_s:ptr_s+2]
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
    
    print("\nTransformation Steps:")
    for step in steps:
        print(step)
    
    return dp[m][n]

def print_dp_table(dp, s, t):
    m = len(s)
    n = len(t)
    
    # Print header
    print("     ε " + ' '.join(f"{c:>3}" for c in t))
    
    # Print first row
    print("ε " + ' '.join(f"{dp[0][j]:>3}" for j in range(n+1)))
    
    # Print remaining rows
    for i in range(1, m+1):
        print(f"{s[i-1]} " + ' '.join(f"{dp[i][j]:>3}" for j in range(n+1)))

# Main execution
s = input("Enter source string: ").strip()
t = input("Enter target string: ").strip()

print("\nCalculating Levenshtein distance with steps...")
distance = levenshtein_distance_with_steps(s, t)
print("\nFinal Levenshtein distance:", distance)
