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
//      random_choice   Randomly choose an arc if there are multiple minimal arcs available.
//      verbose         
//      write_to_file   
//      filename
// OUT  r               -1 == A is not a valid adjacency matrix.
//                      -2 == v is out of range.
//                      -3 == G is not connected.
//                      -4 == Filesystem error.
//
function [r] = prim(A, v, random_choice, verbose, write_to_file, filename)
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
    // `No connection' marking
    nc_weight = max(A) + 1;
    weight = A(v, :);
    visited(v) = 1;
    while (~isempty(find(visited == 0)))
        vertices_not_visited = find(visited == 0);
        effective_weight = find(weight > 0);
        adjs_with_min_weight = intersect( find(weight == min(weight(effective_weight))), ...
            vertices_not_visited);

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

            // It's clear that an arc with zero weight actually doesn't exist.
            if (weight(idx_vertex) == 0) then
                weight(idx_vertex) = nc_weight;
            end
            if ((visited(idx_vertex) == 0) & ...
                (A(idx_curr_vertex, idx_vertex) > 0) & ...
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
