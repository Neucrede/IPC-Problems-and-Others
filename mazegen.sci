funcprot(0);

// Check if a given matrix A is symmetric.
//
function [r] = issymmetric(A)
    if (~issquare(A)) then
        r = 0;
        return;
    end

    if (triu(A) == tril(A)') then
        r = 1;
    else
        r = 0;
    end
endfunction

// Determine the minimal spanning tree of a given connected graph G.
// IN   A               Adjacency matrix of the graph G.
//      v               Initial vertex.
//      random_choice   Randomly choose an edge if there are multiple minimal edges available.
//      verbose         Verbose
//      write_to_file   Write to file
//      filename        Filename
// OUT  r               -1 == A is not a valid adjacency matrix.
//                      -2 == v is out of range.
//                      -3 == G is not connected.
//                      -4 == Filesystem error.
//                      others == Sum of weights.
//
function [r] = prim(A, v, max_weight, random_choice, verbose, write_to_file, filename)
    if (~issymmetric(A)) then
        r = -1;
        return;
    end

    S = size(A);
    N = S(1);
    if ((v < 1) | (v > N)) then
        r = -2;
        return;
    end

    if (write_to_file == 1) then
        [fd, err] = mopen(filename, 'wb');
        if (err ~= 0) then
            r = -4;
            return;
        end
    end

    for i = 1:N
        A(i, i) = 0;
    end

    r = 0;
    visited = zeros(1, N);
    vertices_hash_tab = ones(1, N) .* v;
    weight = A(v, :);
    visited(v) = 1;
    while (~isempty(find(visited == 0)))
        vertices_not_visited = find(visited == 0);
        effective_weights = find(weight < max_weight);
        adjs_with_min_weight = intersect(                   ...
            find(                                           ...        
                weight == min(                              ...
                    weight(                                 ...
                            effective_weights               ...
                    )                                       ...
                )                                           ...
            ), vertices_not_visited                         ...
        );

        if (isempty(adjs_with_min_weight)) then
            r = -3;
            return;
        end

        if (random_choice == 1) then
            idx = grand(1, 1, 'uin', 1, length(adjs_with_min_weight));
        else
            idx = 1;
        end
        idx_curr_vertex = adjs_with_min_weight(idx);

        visited(idx_curr_vertex) = 1;

//        B(vertices_hash_tab(idx_curr_vertex), idx_curr_vertex) = ...
//            A(vertices_hash_tab(idx_curr_vertex), idx_curr_vertex);
//        B(idx_curr_vertex, vertices_hash_tab(idx_curr_vertex)) = ...
//            B(vertices_hash_tab(idx_curr_vertex), idx_curr_vertex);
        r = r + A(vertices_hash_tab(idx_curr_vertex), idx_curr_vertex);
        if (verbose == 1) then
            printf("%d - %d \n", vertices_hash_tab(idx_curr_vertex), idx_curr_vertex);
        end
        if (write_to_file == 1) then
            mfprintf(fd, "%d - %d \n", vertices_hash_tab(idx_curr_vertex), idx_curr_vertex);
        end 

        for idx_vertex = 1:N
            if (idx_vertex == idx_curr_vertex) then
                continue;
            end

            if ((visited(idx_vertex) == 0) & ...
                (A(idx_curr_vertex, idx_vertex) < weight(idx_vertex))) then
                weight(idx_vertex) = A(idx_curr_vertex, idx_vertex);
                vertices_hash_tab(idx_vertex) = idx_curr_vertex;
            end
        end
    end

    if (write_to_file == 1) then
        mclose(fd);
    end
endfunction

function [s] = adj(i, n, d)
    select d
        case 'u' then
            s = i - n;
            if (s <= 0) then
                s = -1;
            end
        case 'd' then
            s = i + n;
            if (s > n.^2) then
                s = -1;
            end
        case 'l' then
            s = i - 1;
            if (modulo(i, n) == 1) then
                s = -1;
            end
        case 'r' then
            s = i + 1;
            if (modulo(i, n) == 0) then
                s = -1;
            end
        else
            s = -1;
    end
endfunction

function [row, col] = getrowcol(n, s)
    c = modulo(s, n);
    if (c == 0) then
        col = n;
    else
        col = c;
    end
    row = (s - col) / n + 1;
endfunction

function [r] = animate_maze(n, mazefile, animate)
    N = n + 1;
    G = ones(2*N+1, 2*N+1);
    for i = 2:2:2*N
        for j = 2:2:2*N
            G(i, j) = 0;
        end
    end

    a = scf();
    if (animate == 1) then
        Matplot(G*10);        
    end

    [fb, err] = mopen(mazefile, 'r');
    if (err ~= 0) then
        r = -1;
        return;
    end

    A = mfscanf(-1, fb, "%d - %d \n");
    U = A(:, 1);
    V = A(:, 2);
    mclose(fb);

    for i = 1:length(U)
        [v_row, v_col] = getrowcol(N, V(i));

        select U(i)
            // U(i) is in the UP direction of V(i)
            case (V(i) - N) then
                G(2*v_row - 1, 2*v_col) = 0;

            // DOWN
            case (V(i) + N) then
                G(2*v_row + 1, 2*v_col) = 0;

            // LEFT
            case (V(i) - 1) then
                G(2*v_row, 2*v_col - 1) = 0;

            // RIGHT
            case (V(i) + 1) then
                G(2*v_row, 2*v_col + 1) = 0;

            else
                r = -2;
                return;
        end

        clf(a);
        if (animate == 1) then
            Matplot(G*10);
            sleep(100);
        end
    end

    clf(a);
    Matplot(G*10);

    r = 0;
endfunction

function [r] = plotmaze(n, mazefile)
    r = animate_maze(n, mazefile, 0);
endfunction

// Generate a square maze pattern.
// Exxxxxxxxxxxxxxxxxxxxxxxxxxxtrmely space-inefficient.
//
function [r] = mazegen(n, mazefn)
    // Let `A' be a sparse matrix
    A = spzeros((n+1).^2, (n+1).^2);

    j = -1;
    for i = 1:(n+1).^2
        j = adj(i, n+1, 'u');
        if (j ~= -1) then
            A(i, j) = 1;
            A(j, i) = 1;
        end

        j = adj(i, n+1, 'd');
        if (j ~= -1) then
            A(i, j) = 1;
            A(j, i) = 1;
        end

        j = adj(i, n+1, 'l');
        if (j ~= -1) then
            A(i, j) = 1;
            A(j, i) = 1;
        end

        j = adj(i, n+1, 'r');
        if (j ~= -1) then
            A(i, j) = 1;
            A(j, i) = 1;
        end
    end

    disp(A);
    pause;
    start = grand(1, 1, 'uin', 1, (n+1).^2);
    r = prim(A, start(1), 1, 1, 1, mazefn);
    
    // animate_maze(n, mazefn, animate);
endfunction

