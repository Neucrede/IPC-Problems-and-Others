function stemplot(n, x)
    plot(n, x, 'o');
    f = gcf();
    f.children.children(1).children.line_mode = "off";
    f.children.children(1).children.polyline_style = 3;
    f.children.children(1).children.mark_mode = "on";
    f.children.children(1).children.mark_style = 9;
    f.children.children(1).children.mark_size = 6;
endfunction
