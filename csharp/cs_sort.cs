/*
 Generic sorting study
 Generic Sorting in C# (redo of Assignment 5)

 Author: @forgehk

 Tools / sites I used while doing this assignment:
   - .NET SDK / C# compiler (csc or dotnet)
   - Visual Studio Code (editor)
   - Windows Terminal / Ubuntu terminal
   - learn.microsoft.com (List<T>.Sort documentation)
*/

using System;
using System.Collections.Generic;

//simple class to hold a person's name and age
class Person
{
    public string Name;
    public int Age;
}

class Program
{
    static void Main()
    {
        // original data for the three sort tasks -----

        // floating point numbers we need to sort in ascending order
        List<double> numbers = new List<double>
        {
            645.41, 37.59, 76.41, 5.31, -34.23, 1.11,
            1.10, 23.46, 635.47, -876.32, 467.83, 62.25
        };

        // List of people (name, age) we will sort in two different ways
        List<Person> people = new List<Person>
        {
            new Person { Name = "Hal",       Age = 20 },
            new Person { Name = "Susann",    Age = 31 },
            new Person { Name = "Dwight",    Age = 19 },
            new Person { Name = "Kassandra", Age = 21 },
            new Person { Name = "Lawrence",  Age = 25 },
            new Person { Name = "Cindy",     Age = 22 },
            new Person { Name = "Cory",      Age = 27 },
            new Person { Name = "Mac",       Age = 19 },
            new Person { Name = "Romana",    Age = 27 },
            new Person { Name = "Doretha",   Age = 32 },
            new Person { Name = "Danna",     Age = 20 },
            new Person { Name = "Zara",      Age = 23 },
            new Person { Name = "Rosalyn",   Age = 26 },
            new Person { Name = "Risa",      Age = 24 },
            new Person { Name = "Benny",     Age = 28 },
            new Person { Name = "Juan",      Age = 33 },
            new Person { Name = "Natalie",   Age = 25 }
        };

        // (a): sort numbers in ascending order -----
        // list<T>.Sort uses the natural ordering of double (smallest to largest)
        numbers.Sort();

        Console.WriteLine("Numbers asc:");
        for (int i = 0; i < numbers.Count; i++)
        {
            if (i > 0) Console.Write(", ");
            // "F2" formats the number with 2 decimal places
            Console.Write(numbers[i].ToString("F2"));
        }
        Console.WriteLine();
        Console.WriteLine();

        // (b): sort people by name (ascending) -----

        // Copy of the original list so we can do a separate sort by name
        List<Person> byName = new List<Person>(people);

        // Sort by Name using a lambda expression as the comparison
        byName.Sort((a, b) => string.Compare(a.Name, b.Name, StringComparison.Ordinal));

        Console.WriteLine("People by name asc:");
        for (int i = 0; i < byName.Count; i++)
        {
            if (i > 0) Console.Write(" | ");
            Console.Write($"{byName[i].Name}, {byName[i].Age}");
        }
        Console.WriteLine();
        Console.WriteLine();

        // (c): sort people by age (descending),
        //and for ties use name ascending as tie-breaker

        List<Person> byAge = new List<Person>(people);

        //custom comparison: higher Age first; if same Age, compare Name alphabetically
        byAge.Sort((a, b) =>
        {
            int ageCompare = b.Age.CompareTo(a.Age); // age descending
            if (ageCompare != 0)
            {
                return ageCompare;
            }
            //same age: sort alphabetically by name
            return string.Compare(a.Name, b.Name, StringComparison.Ordinal);
        });

        Console.WriteLine("People by age desc (tie => name asc):");
        for (int i = 0; i < byAge.Count; i++)
        {
            if (i > 0) Console.Write(" | ");
            Console.Write($"{byAge[i].Name}, {byAge[i].Age}");
        }
        Console.WriteLine();
    }
}
